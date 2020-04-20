#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "parameters.h"
#include "merge.c"

#define N 2048



// only used for synchronizing stdout from overlap.



int main()
{
    system("clear");
    int *data = malloc(N * sizeof(*data));
    int *dup = malloc(N* sizeof(*data));
    unsigned int i;

    srand((unsigned)time(0));
    for (i=0; i<N; ++i)
    {
        data[i] = rand() % 10000;
	dup[i]=data[i];
    }
    merge_sort(data, N);

    printf("\nIn intervals of 256 before sorting\n%d\t", dup[0]);
    for(i=255;i<N-256;i+=256)
	printf("%d, %d\t", dup[i], dup[i+1]);
    printf("%d\n\n", dup[N-1]);

    printf("In intervals of 256 after sorting\n%d\t", data[0]);
    for(i=255; i<N-256;i+=256)
	printf("%d, %d\t", data[i], data[i+1]);
    printf("%d\n\n", data[N-1]);

    char s;
    printf("\nPrint the entire Array? ");
    scanf("%c", &s);
    if(s=='Y' || s=='y')
    {
	printf("\n\nBEFORE SORTING\n\n");
        for(i=0;i<N;++i)
            printf("%4d ", dup[i]);

        printf("\n\nAFTER SORTING\n\n");
        for(i=0;i<N;++i)
            printf("%4d ", data[i]);
        printf("\n\n");
    }
    free(dup);
    free(data);
    return 0;

}

