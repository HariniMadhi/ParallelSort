#ifndef MERGE_H
#define MERGE_H

#include "parameter.h"
#include <pthread.h>
#include <stdlib.h>

extern void *merge_sort_thread (void *);
extern void merge(int *, int *, int *, int, int, int);
extern void merge_sort_mt(int *, size_t, int, int , int);
extern void merge_sort(int *, size_t);

#endif
