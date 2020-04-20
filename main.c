#include <stdio.h>
#include <stdlib.h>
#include "merge.h"
#include <time.h>

int main()
{
    system("clear");	//system("cls") in Windows, system("clear") in Linux, for clearing the terminal before printing

    int N;
    printf("Enter the number of elements in the Array: ");
    scanf("%d", &N);	//User input

    int *data = malloc(N * sizeof(*data));	//Memory allocation for N integers that will be sorted
    int *dup = malloc(N* sizeof(*data));	//Memory allocation for a duplicate array to store the original unsorted N integers
    int i;

    srand(time(NULL));				//Random numbers are generated for inputs, to ensure mix up

    for(i=0; i<N; ++i)
    {
        data[i] = rand() % 10000;		//Input
        dup[i]=data[i];
    }


    merge_sort(data, N);			//Function included from merge.h, initialises the mergesort


    int n=N/8;
    printf("\n\n\nIn intervals of %d before sorting\n\n%d\t", n,dup[0]);
    for(i=n-1;i<N-n;i+=n)
        printf("%d, %d\t", dup[i], dup[i+1]);
    printf("%d\n\n", dup[N-1]);
									//To verify the split operation, and compare
    printf("In intervals of %d after sorting\n\n%d\t",n, data[0]);
    for(i=n-1; i<N-n;i+=n)
        printf("%d, %d\t", data[i], data[i+1]);
    printf("%d\n\n", data[N-1]);


    char s;
    printf("\nPrint the entire Array? ");
    scanf("%*c%c", &s);
    if(s=='Y' || s=='y')
    {
        printf("\n\nBEFORE SORTING\n\n");		//Duplicate array usage
        for(i=0;i<N;++i)
            printf("%-4d ", dup[i]);

        printf("\n\nAFTER SORTING\n\n");
        for(i=0;i<N;++i)
            printf("%-4d ", data[i]);
        printf("\n\n");
    }


    free(dup);
    free(data);
    return 0;

}

