#ifndef OBSCENEQSORT_H_INCLUDED
#define OBSCENEQSORT_H_INCLUDED

//#include <alloca.h>
#ifndef __compar_fn_t
typedef int (*__compar_fn_t) (const void *, const void *);
#endif // __compar_fn_t
#ifndef __compar_d_fn_t
typedef int (*__compar_d_fn_t)(const void *, const void *, void *);
#endif // __compar_fn_t

#include <limits.h>
#include <stdlib.h>
#include <string.h>

void OBC_quicksort (void *const pbase, size_t total_elems, size_t size, int OBC_TYPE);

void OBC_introSortFunc(unsigned char *arr, unsigned long long int length);

void OBC_Introsort2(unsigned char arr[], unsigned char *begin, unsigned char *end);
void Introsort(int arr[], int *begin, int *end);

#endif // OBSCENEQSORT_H_INCLUDED
