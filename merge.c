#include <math.h>
#include <string.h>
#include <unistd.h>
#include "merge.h"

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;        //For synchronizing Output
int DEPTH = 8;

void gotoxy(int x, int y)		//Function to move cursor to column x, row y, for formatting output
{
	printf("%c[%d;%df", 0x1B, y, x);
}

void merge(int *start, int *mid, int *end, int depth, int threadspace, int flag)	//Traditional merge function
{
	int *res = malloc((end - start)*sizeof(*res));	//Auxillary memory, sum of the size of the two partitions to be merged
    	int *lhs = start, *rhs = mid, *dst = res;

    	while (lhs != mid && rhs != end)
        	*dst++ = (*lhs < *rhs) ? *lhs++ : *rhs++;

    	while (lhs != mid)
        	*dst++ = *lhs++;

    	// copy results
    	memcpy(start, res, (rhs - start) * sizeof *res);
	free(res); 	//Free the auxillary space

    	if(flag)	//Printing the starting and ending of the merged array, formatted
    	{

        	int i=threadspace*16, j=log(depth)/log(2) * 2 +DEPTH +4;
        	pthread_mutex_lock(&mtx);
		gotoxy(i, j);
		printf("%4d .. %4d", start[0], start[end-start -1]);
		pthread_mutex_unlock(&mtx);
    	}

}

void merge_sort_mt(int *start, size_t len, int depth, int threadspace, int flag)	//Function that divides, assigns thread, and merges
{
	if (len < 2)
        	return;
    	if(flag)	//Printing the starting and ending of the array that needs too be divided, formatted
    	{
		int i=threadspace*16, j=DEPTH-2*(log(depth)/log(2)) + 2;
		pthread_mutex_lock(&mtx);
		gotoxy(i,j);
		printf("%4d .. %4d", *start, *(start+len-1));
		pthread_mutex_unlock(&mtx);
    	}

    	if(depth==1||len<4)
    	{							//If the Depth goes below 1, threads will not be created, and will be recursed instead
        	merge_sort_mt(start, len/2, 1, 0, 0);		//flag is used to determine this
        	merge_sort_mt(start+len/2, len-len/2, 1, 0, 0);
    	}
    	else
    	{
        	struct parameter params = { start, len/2, depth/2, 2*threadspace , 1 };	//Creating parameter, to be sent as argument
        	pthread_t thrd;

        	// create our thread
        	pthread_create(&thrd, NULL, merge_sort_thread, &params);

        	// recurse into our top-end parition
        	merge_sort_mt(start+len/2, len-len/2, depth/2, 2*threadspace +1, 1 );

        	// join on the launched thread ---- Synchronisation
        	pthread_join(thrd, NULL);
    	}

    	// merge the partitions.
    	merge(start, start+len/2, start+len, depth, threadspace, flag);

}

void *merge_sort_thread(void *pv)
{
	struct parameter *params = (struct parameter *) pv;	//Cast into parameter, to be sent into merge_sort_mt
    	merge_sort_mt(params->start, params->len, params->depth, params->threadspace, params->flag);
    	return pv;

}

void merge_sort(int *start, size_t len)				//Initial call for the merge_sort_mt
{
    merge_sort_mt(start, len, DEPTH, 0, 1);			//This function abstracts other functions, and will be used in main.c
}

