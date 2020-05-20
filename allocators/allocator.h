#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include "../obc.h"
#include "../containers/ray.h"
#include "allocators.h"

typedef struct OBC_Allocator{
    OBC_Ray backed;
    unsigned int metaUnits;
    OBC_Ray meta[OBC_ALLOC_META_ADDRESSING+1];
}OBC_Allocator;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newAllocator(size_t unitSize);
void *OBC_initAllocator(OBC_Allocator *allocator, size_t unitSize);

void OBC_freeAllocator(void *allocator);
void OBC_freeAllocatorData(OBC_Allocator *allocator);


size_t OBC_AllocatorMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocatorFree(void *allocator, size_t pos);

/*************************************
Alternate Utilities
*************************************/

#define OBC_AllocatorGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocatorGetValue(rawPtr, index) ((*(rawPtr))[index])


/*************************************
Internal Utilities
*************************************/

#define _OBC_ALLOCATOR_PTR_CAST(rawPtr) ((OBC_Allocator *)(rawPtr))
#define _OBC_ALLOCATOR_OFFSET ((size_t)(&((OBC_Allocator *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCATOR(rawPtr) (_OBC_ALLOCATOR_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCATOR_OFFSET))

OBC_ERROR_ENUM OBC_AllocatorMarkAllocated(void *allocator, size_t pos);
OBC_ERROR_ENUM OBC_AllocatorExpand(void *allocator);
size_t OBC_AllocatorGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocatorMarkFreed(void *allocator, size_t pos);


/*************************************
======================================
*************************************/

size_t OBC_metaFirst0Bit(OBC_ALLOC_META_TYPE *data, size_t unitsToCheck);

OBC_ERROR_ENUM OBC_metaMarkBit1__(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit0__(OBC_ALLOC_META_TYPE *meta, const size_t pos);

OBC_ERROR_ENUM OBC_metaMarkBit1(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit0(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit(OBC_ALLOC_META_TYPE *meta, const size_t pos, const char bitVal);
size_t OBC_metaFirstEmptyBit(OBC_ALLOC_META_TYPE *meta, size_t metaUnitLength);

#endif // ALLOCATOR_H_INCLUDED
