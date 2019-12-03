#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include "OBC.h"
#include "allocators.h"
#include "ray.h"

#define _OBC_ALLOCATOR2_PTR_CAST(rawPtr) ((OBC_Allocator2 *)(rawPtr))
#define _OBC_ALLOCATOR2_OFFSET ((size_t)(&((OBC_Allocator2 *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCATOR2(rawPtr) (_OBC_ALLOCATOR2_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCATOR2_OFFSET))
typedef struct OBC_Allocator2{
    OBC_Ray backed;
    unsigned int metaUnits;
    OBC_Ray meta[OBC_ALLOC_META_ADDRESSING+1];
}OBC_Allocator2;


OBC_ERROR_ENUM OBC_metaMarkBit1(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit0(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit(OBC_ALLOC_META_TYPE *meta, const size_t pos, const char bitVal);
size_t OBC_metaFirstEmptyBit(OBC_ALLOC_META_TYPE *meta, size_t metaUnitLength);

#define OBC_Allocator2GetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_Allocator2GetValue(rawPtr, index) ((*(rawPtr))[index])



size_t OBC_metaFirst0Bit(OBC_ALLOC_META_TYPE *data, size_t unitsToCheck);
void **OBC_newAllocator2(size_t unitSize);
void *OBC_initAllocator2(OBC_Allocator2 *allocator, size_t unitSize);

void OBC_freeAllocator2(void *allocator);
void OBC_freeAllocator2Data(void *allocator);



OBC_ERROR_ENUM OBC_Allocator2MarkAllocated3(void *allocator, size_t pos);
OBC_ERROR_ENUM OBC_Allocator2Expand3(void *allocator);
size_t OBC_Allocator2GetFreeLocation3(void *allocator);
size_t OBC_Allocator2Malloc3(void *allocator);
OBC_ERROR_ENUM OBC_Allocator2Free3(void *allocator, size_t pos);
OBC_ERROR_ENUM OBC_Allocator2MarkFreed3(void *allocator, size_t pos);

OBC_ERROR_ENUM OBC_metaMarkBit1__(OBC_ALLOC_META_TYPE *meta, const size_t pos);
OBC_ERROR_ENUM OBC_metaMarkBit0__(OBC_ALLOC_META_TYPE *meta, const size_t pos);

#endif // ALLOCATOR_H_INCLUDED
