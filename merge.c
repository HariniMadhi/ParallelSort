#ifndef MERGE_C
#define MERGE_C

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "parameters.h"

int DEPTH = 4;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
// forward declare our thread proc
void *merge_sort_thread(void *pv);

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}


// a simple merge algorithm. there are *several* more efficient ways
//  of doing this, but the purpose of this exercise is to establish
//  merge-threading, so we stick with simple for now.
void merge(int *start, int *mid, int *end, int depth, int threadspace, int flag)
{
    int *res = malloc((end - start)*sizeof(*res));
    int *lhs = start, *rhs = mid, *dst = res;

    while (lhs != mid && rhs != end)
        *dst++ = (*lhs < *rhs) ? *lhs++ : *rhs++;

    while (lhs != mid)
        *dst++ = *lhs++;

    // copy results
    memcpy(start, res, (rhs - start) * sizeof *res);
    if(flag)
    {
        pthread_mutex_lock(&mtx);
        gotoxy(0, 15);
        int i=0;
        for(i=0;i<depth;i++)
	    printf("\n\n");
	for(i=0;i<threadspace;i++)
	    printf("\t\t");
	printf("%4d .. %4d", res[0], res[rhs-start -1 ]);
        sleep(1);
	pthread_mutex_unlock(&mtx);
    }
    free(res);
}

// our multi-threaded entry point.
void merge_sort_mt(int *start, size_t len, int depth, int threadspace, int flag)
{
    if (len < 2)
        return;
    if(flag)
    {
		pthread_mutex_lock(&mtx);
		int i;
		gotoxy(0,3);
		for(i=0;i<DEPTH-depth;i++)
			printf("\n\n");
		for(i=0;i<threadspace;i++)
			printf("\t\t");
		printf("%4d .. %4d", *start, *(start+len-1));
		sleep(1);
		pthread_mutex_unlock(&mtx);
    }

    if(depth==0||len<4)
    {
        merge_sort_mt(start, len/2, 0, 0, 0);
        merge_sort_mt(start+len/2, len-len/2, 0, 0, 0);
    }
    else
    {
        struct Params params = { start, len/2, depth/2, 2*threadspace , 1 };
        pthread_t thrd;

        // create our thread
        pthread_create(&thrd, NULL, merge_sort_thread, &params);

        // recurse into our top-end parition
        merge_sort_mt(start+len/2, len-len/2, depth/2, 2*threadspace +1, 1 );

        // join on the launched thread
        pthread_join(thrd, NULL);

    }

    // merge the partitions.
    merge(start, start+len/2, start+len, depth, threadspace, flag);
}

// our thread-proc that invokes merge_sort. this just passes the
//  given parameters off to our merge_sort algorithm
void *merge_sort_thread(void *pv)
{
    struct Params *params = (struct Params*) pv;
    merge_sort_mt(params->start, params->len, params->depth, params->threadspace, params->flag);
    return pv;
}

// public-facing api
void merge_sort(int *start, size_t len)
{
    merge_sort_mt(start, len, DEPTH, 0, 1); // 4 is a nice number, will use 7 threads.
}

#endif