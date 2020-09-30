#ifndef ALLOCRAY_H_INCLUDED
#define ALLOCRAY_H_INCLUDED

#include "../obc.h"
#include "../containers/ray.h"
#include "allocators.h"

typedef struct OBC_AllocRay{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocRay;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newAllocRay(size_t unitSize);
OBC_ERROR_ENUM OBC_initAllocRay(OBC_AllocRay *allocRay, size_t unitSize);

void **OBC_AllocRayGetDataPointer(OBC_AllocRay *allocRay);

void OBC_freeAllocRay(void *allocator);
void OBC_freeAllocRayData(OBC_AllocRay *allocRay);

OBC_Offset OBC_AllocRayMalloc(void *allocator);
OBC_Offset OBC_AllocRayMallocRaw(OBC_AllocRay *allocRay);

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, OBC_Offset index);
OBC_ERROR_ENUM OBC_AllocRayFreeRaw(OBC_AllocRay *allocRay, OBC_Offset index);




/*************************************
Internal Utilities
*************************************/

#define _OBC_ALLOCRAY_PTR_CAST(rawPtr) ((OBC_AllocRay *)(rawPtr))
#define _OBC_ALLOCRAY_OFFSET ((size_t)(&((OBC_AllocRay *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCRAY(rawPtr) (_OBC_ALLOCRAY_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCRAY_OFFSET))
#define OBC_FROM_RAW_ALLOCRAY(rawPtr) ((_OBC_ALLOCRAY_PTR_CAST(((void*)(rawPtr))+ (_OBC_ALLOCRAY_OFFSET)) ))



OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayExpandRaw(OBC_AllocRay *allocRay);

OBC_Offset OBC_AllocRayGetFreeLocation(void *allocator);
OBC_Offset OBC_AllocRayGetFreeLocationRaw(OBC_AllocRay *allocRay);

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, OBC_Offset index);
OBC_ERROR_ENUM OBC_AllocRayMarkFreedRaw(OBC_AllocRay *allocRay, OBC_Offset index);

OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, OBC_Offset index);
OBC_ERROR_ENUM OBC_AllocRayMarkAllocatedRaw(OBC_AllocRay *allocRay, OBC_Offset index);

int OBC_AllocRayFindFirstEmptyBit(OBC_ALLOC_META_TYPE rraw);

#endif // ALLOCRAY_H_INCLUDED
