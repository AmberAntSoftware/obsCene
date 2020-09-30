#ifndef ALLOCLIST_H_INCLUDED
#define ALLOCLIST_H_INCLUDED

#include "../obc.h"
#include "../containers/ray.h"
#include "allocators.h"

typedef struct OBC_AllocList{

    OBC_Ray backed;
    OBC_Ray meta;

}OBC_AllocList;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newAllocList(size_t unitSize);
OBC_ERROR_ENUM OBC_initAllocList(OBC_AllocList *allocator, size_t unitSize);

void **OBC_AllocListGetDataPointer(OBC_AllocList *allocator);

void OBC_freeAllocList(void *allocator);
void OBC_freeAllocListData(OBC_AllocList *allocator);

OBC_Offset OBC_AllocListMalloc(void *allocator);
OBC_Offset OBC_AllocListMallocRaw(OBC_AllocList *allocator);

OBC_ERROR_ENUM OBC_AllocListFree(void *allocator, OBC_Offset data);
OBC_ERROR_ENUM OBC_AllocListFreeRaw(OBC_AllocList *allocator, OBC_Offset data);

/*************************************
Alternate Utilities
*************************************/

/*
size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastExpand(void *allocator);

///UTILITY
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocFastMarkAllocated(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocFastMarkFreed(void *allocator, size_t pos);
*/
#define OBC_AllocListGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocListGetValue(rawPtr, index) ((*(rawPtr))[index])

/*************************************
Internal Utilities
*************************************/

#define _OBC_ALLOCLIST_PTR_CAST(rawPtr) ((OBC_AllocList *)(rawPtr))
#define _OBC_ALLOCLIST_OFFSET ((size_t)(&((OBC_AllocList *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCLIST(rawPtr) (_OBC_ALLOCLIST_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCLIST_OFFSET))
#define OBC_FROM_RAW_ALLOCLIST(rawPtr) ((_OBC_ALLOCLIST_PTR_CAST(((void*)(rawPtr))+ (_OBC_ALLOCLIST_OFFSET)) ))

#define OBC_ALLOCLIST_META OBC_Offset



#endif
