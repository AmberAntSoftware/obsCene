#ifndef SETHASH_H_INCLUDED
#define SETHASH_H_INCLUDED

#include "../obc.h"
#include "../allocators/alloclistbit.h"

typedef struct OBC_SetHash{

    OBC_AllocListBit values;
    OBC_Ray keys;

    size_t buckets;
    size_t count;

}OBC_SetHash;

/*************************************
Initialization / Deallocation
*************************************/




/*************************************
Internal Utilities
*************************************/

#define _OBC_HASHMAP_PTR_CAST(rawPtr) ((OBC_Dequeue *)(rawPtr))
#define _OBC_HASHMAP_OFFSET ((size_t)(&((OBC_Dequeue *)NULL)->queue.backed.rawData))
#define OBC_TO_HASHMAP_PTR(rawPtr) (_OBC_HASHMAP_PTR_CAST(((void*)(rawPtr)) - _OBC_HASHMAP_OFFSET))

#define OBC_FROM_HASHMAP_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_HASHMAP_OFFSET))
#define OBC_FROM_HASHMAP_VAL(hashmapVal) ((void**)(((void*)(&(hashmapVal))) + _OBC_HASHMAP_OFFSET))


#endif // SETHASH_H_INCLUDED
