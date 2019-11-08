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

/*
typedef struct OBC_Allocator{

    size_t (*calloc)(void** self);
    size_t (*malloc)(void** self);
    size_t (*free)(void** self, size_t unit);

}OBC_Allocator;
*/

typedef struct OBC_AllocRay{

    OBC_Ray backed;
    OBC_Ray meta;
    size_t metaBits;

}OBC_AllocRay;

///returns the starting location of the data -1 unitSize
void **OBC_newAllocRay(size_t unitSize);
void *OBC_initAllocRay(void *allocator, size_t unitSize);

size_t OBC_AllocRayMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);

size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data);
OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator);



///utility -- finds first empty leftmost bit of value
int OBC_FindFirstEmptyBit(OBC_ALLOCATOR_META_TYPE rraw);
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

#endif // ALLOCATOR_H_INCLUDED
