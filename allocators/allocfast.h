#ifndef ALLOCFAST_H_INCLUDED
#define ALLOCFAST_H_INCLUDED

#include "OBC.h"
#include "allocators.h"
#include "ray.h"

#define _OBC_ALLOCFAST_PTR_CAST(rawPtr) ((OBC_AllocFast *)(rawPtr))
#define _OBC_ALLOCFAST_OFFSET ((size_t)(&((OBC_AllocFast *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCFAST(rawPtr) (_OBC_ALLOCFAST_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCFAST_OFFSET))
#define OBC_FROM_RAW_ALLOCFAST(rawPtr) ((_OBC_ALLOCFAST_PTR_CAST(((void*)(rawPtr))+ (_OBC_ALLOCFAST_OFFSET)) ))

#define OBC_ALLOCFAST_META size_t

typedef struct OBC_AllocFast{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocFast;


void **OBC_newAllocFast(size_t unitSize);
void *OBC_initAllocFast(OBC_AllocFast *allocator, size_t unitSize);
void OBC_freeAllocFast(void *allocator);
void OBC_freeAllocFastData(void *allocator);

size_t OBC_AllocFastMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastFree(void *allocator, size_t data);

size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastExpand(void *allocator);

///UTILITY
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocFastMarkAllocated(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocFastMarkFreed(void *allocator, size_t pos);

#define OBC_AllocFastGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocFastGetValue(rawPtr, index) ((*(rawPtr))[index])




#endif // ALLOCFAST_H_INCLUDED
