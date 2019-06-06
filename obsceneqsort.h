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

void OBC_quicksort (void *const pbase, size_t total_elems, size_t size, __compar_d_fn_t cmp, void *userStorage);

#endif // OBSCENEQSORT_H_INCLUDED
