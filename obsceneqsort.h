#ifndef OBSCENEQSORT_H_INCLUDED
#define OBSCENEQSORT_H_INCLUDED

#include "obsceneinternals.h"

//#include <alloca.h>
#ifndef __compar_fn_t
typedef int (*__compar_fn_t) (const void *, const void *);
#endif // __compar_fn_t
#ifndef __compar_d_fn_t
typedef int (*__compar_d_fn_t)(const void *, const void *, void *);
#endif // __compar_fn_t

void OBC_quicksort (void *const pbase, size_t total_elems, size_t size, OBC_ENUM OBC_TYPE);

#endif // OBSCENEQSORT_H_INCLUDED
