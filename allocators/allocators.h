#ifndef ALLOCATORS_H_INCLUDED
#define ALLOCATORS_H_INCLUDED

#include "OBC.h"

#define OBC_ALLOC_MAX_CONTRACT_TRIES 3
#define OBC_ALLOC_META_TYPE size_t
///CHUNKSIZE must be a power of 2
#define OBC_ALLOC_META_CHUNKSIZE 8
#define OBC_ALLOC_META_CHUNKSIZE_BITS (OBC_ALLOC_META_CHUNKSIZE*OBC_ALLOC_META_BITS)
//#define OBC_ALLOCATOR_META_CHUNKSIZE_MOD(position) ((position)&1024))
#define OBC_ALLOC_META_BYTES (sizeof(OBC_ALLOC_META_TYPE))
#define OBC_ALLOC_META_BITS ((sizeof(OBC_ALLOC_META_TYPE))*CHAR_BIT)
#define OBC_ALLOC_META_MASK ((OBC_ALLOC_META_TYPE)(1 << (sizeof(OBC_ALLOC_META_TYPE)*CHAR_BIT-1)))


///meta addressing is N units to reach approximately size of 2^64
///through lookups -- current is 32bit ish of 1k, 4 is ~2^64
///mutate as needed with other above chunk and comparison size optimizations
#define OBC_ALLOC_META_ADDRESSING 5

///set extras to 0 past size_t maximum value
#define OBC_ALLOC_UNITS_0 (OBC_ALLOC_META_CHUNKSIZE_BITS)
#define OBC_ALLOC_UNITS_1 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_0)
#define OBC_ALLOC_UNITS_2 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_1)
#define OBC_ALLOC_UNITS_3 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_2)
#define OBC_ALLOC_UNITS_4 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_3)
#define OBC_ALLOC_UNITS_5 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_4)
///manually keep in synch with unit limits of addressing
#define OBC_ALLOC_META_LIMITS \
{\
    OBC_ALLOC_UNITS_0,OBC_ALLOC_UNITS_1 \
    ,OBC_ALLOC_UNITS_2,OBC_ALLOC_UNITS_3 \
    ,OBC_ALLOC_UNITS_4/*,OBC_ALLOC_UNITS_5,*/ \
};


typedef struct OBC_AllocatorPointer{
    void **allocator;
    size_t where;
}OBC_AllocatorPointer;

#endif // ALLOCATORS_H_INCLUDED
