#ifndef PARAMETER_H
#define	PARAMETER_H

#include<stdio.h>
/*
	This structure is for passing necessary arguments to the merge sort functions, executed
	as threads.
*/

struct parameter
{
	int* start;		//Points to the starting of the memory location under process
	size_t len;		//Stores the length of the Memory under process
	int depth;		//Level of split in the array
	int threadspace;	//Partition number
	int flag;		//1, if partitioned using threads
};

#endif
