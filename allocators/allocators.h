#ifndef ALLOCATORS_H_INCLUDED
#define ALLOCATORS_H_INCLUDED

#include "OBC.h"

#define OBC_ALLOCATOR_MAX_CONTRACT_TRIES 3
#define OBC_ALLOCATOR_META_TYPE unsigned int
#define OBC_ALLOCATOR_META_CHUNKSIZE 1023
#define META_BITS (sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8)
#define META_MASK ((OBC_ALLOCATOR_META_TYPE)(1 << (sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8-1)))
//meta addressing is N units to reach approximately size of 2^64
//through lookups -- current is 32bit ish of 1k, 4 is ~2^64
#define OBC_ALLOCATOR_META_ADDRESSING 4


#define OBC_ALLOC_UNITS_0 OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS;
#define OBC_ALLOC_UNITS_1 OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_0;
#define OBC_ALLOC_UNITS_2 OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_1;
#define OBC_ALLOC_UNITS_3 OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_2;

/*
typedef OBC_AllocMeta{

}
*/

/*
typedef struct OBC_Allocator{

    size_t (*calloc)(void** self);
    size_t (*malloc)(void** self);
    size_t (*free)(void** self, size_t unit);

}OBC_Allocator;
*/

size_t OBC_findFirstLeftBit(OBC_ALLOCATOR_META_TYPE *meta);


#endif // ALLOCATORS_H_INCLUDED
