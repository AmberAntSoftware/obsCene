#ifndef ALLOCRAY_H_INCLUDED
#define ALLOCRAY_H_INCLUDED

#include "../obc.h"
#include "../containers/ray.h"
#include "allocators.h"

#define _OBC_ALLOCRAY_PTR_CAST(rawPtr) ((OBC_AllocRay *)(rawPtr))
#define _OBC_ALLOCRAY_OFFSET ((size_t)(&((OBC_AllocRay *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCRAY(rawPtr) (_OBC_ALLOCRAY_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCRAY_OFFSET))
#define OBC_FROM_RAW_ALLOCRAY(rawPtr) ((_OBC_ALLOCRAY_PTR_CAST(((void*)(rawPtr))+ (_OBC_ALLOCRAY_OFFSET)) ))

typedef struct OBC_AllocRay{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocRay;

///returns the starting location of the data -1 unitSize
void **OBC_newAllocRay(size_t unitSize);
void *OBC_initAllocRay(OBC_AllocRay *allocator, size_t unitSize);
void OBC_freeAllocRay(void *allocator);
void OBC_freeAllocRayData(void *allocator);

size_t OBC_AllocRayMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);

size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator);

///UTILITY
///finds first empty leftmost bit of value
int OBC_AllocRayFindFirstEmptyBit(OBC_ALLOC_META_TYPE rraw);
///UTILITY
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos);


#endif // ALLOCRAY_H_INCLUDED
