#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include <string.h>

#include "OBC.h"
#include "allocators.h"

#include "ray.h"

#define OBC_ALLOC_SMALL_OBT_BYTES 3000


#define _OBC_ALLOCRAY_PTR_CAST(rawPtr) ((OBC_AllocRay *)(rawPtr))
#define _OBC_ALLOCRAY_OFFSET ((size_t)(&((OBC_AllocRay *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCRAY(rawPtr) (_OBC_ALLOCRAY_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCRAY_OFFSET))
#define OBC_FROM_RAW_ALLOCRAY(rawPtr) ((_OBC_ALLOCRAY_PTR_CAST((rawPtr)) + (_OBC_ALLOCRAY_OFFSET)))

//typedef OBC_Ray OBC_Meta;

typedef struct OBC_AllocRay{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocRay;

///returns the starting location of the data -1 unitSize
void **OBC_newAllocRay(size_t unitSize);
void *OBC_initAllocRay(void *allocator, size_t unitSize);
void OBC_freeAllocRay(void *allocator);
void OBC_freeAllocRayData(void *allocator);

size_t OBC_AllocRayMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);

size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);
OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator);



///utility -- finds first empty leftmost bit of value
int OBC_FindFirstEmptyBit(OBC_ALLOC_META_TYPE rraw);
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos);
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos);

///
///
///
///
///
///
///
///
///

typedef struct OBC_AllocFast{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocFast;

void **OBC_newAllocFast(size_t unitSize);
void *OBC_initAllocFast(void *allocator, size_t unitSize);

//size_t OBC_AllocAllocFast(void *allocator);
//OBC_ERROR_ENUM OBC_AllocAllocFast(void *allocator, size_t data);


/*
void **OBC_newAllocRay(size_t unitSize, void (*freeFunction)(void *value));
void *OBC_initAllocRay(void *allocator, size_t unitSize, void (*freeFunction)(void *value));
size_t OBC_AllocRayMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);
size_t OBC_AllocRayGetFreeLocation(void *allocator);
int OBC_FindFirstEmptyBit(OBC_ALLOCATOR_META_TYPE rraw);
OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos);
OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos);
*/





#define _OBC_ALLOCATOR_PTR_CAST(rawPtr) ((OBC_Allocator *)(rawPtr))
#define _OBC_ALLOCATOR_OFFSET ((size_t)(&((OBC_Allocator *)NULL)->rawAlloc.backed.rawData))
#define OBC_TO_RAW_ALLOCATOR(rawPtr) (_OBC_ALLOCATOR_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCATOR_OFFSET))

typedef struct OBC_Allocator{
    OBC_AllocRay rawAlloc;
    unsigned int metaUnits;
    OBC_Ray meta[OBC_ALLOC_META_ADDRESSING];
}OBC_Allocator;

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





void **OBC_newAllocator(size_t unitSize);
void *OBC_initAllocator(void *allocator, size_t unitSize);
void OBC_freeAllocator(void *allocator);
void OBC_freeAllocatorData(void *allocator);

size_t OBC_AllocatorMalloc(void *allocator);

size_t OBC_AllocatorGetFreeLocation(void *allocator);

OBC_ERROR_ENUM OBC_AllocatorMarkAllocated(void *allocator);
OBC_ERROR_ENUM OBC_AllocatorMarkFreed(void *allocator);




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
