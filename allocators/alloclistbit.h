#ifndef ALLOCLISTBIT_H_INCLUDED
#define ALLOCLISTBIT_H_INCLUDED

#include "../containers/ray.h"

#define OBC_ALLOCLISTBIT_META OBC_Offset
typedef struct OBC_AllocListBit{

    OBC_Ray backed;

    OBC_ALLOCLISTBIT_META metaCache;
    OBC_Offset metaCachePos;
    OBC_Offset maxHead;

    OBC_Ray meta;

}OBC_AllocListBit;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newAllocListBit(size_t unitSize);
OBC_ERROR_ENUM OBC_initAllocListBit(OBC_AllocListBit *allocator, size_t unitSize);
OBC_ERROR_ENUM OBC_initAllocListBitComplex(OBC_AllocListBit *allocator, OBC_Offset initialReserveCount , size_t unitSize);

void OBC_freeAllocListBit(void *allocator);
void OBC_freeAllocListBitData(OBC_AllocListBit *allocator);

void **OBC_AllocListBitGetDataPointer(OBC_AllocListBit *allocator);

OBC_Offset OBC_AllocListBitMalloc(void *allocator);
OBC_Offset OBC_AllocListBitMallocRaw(OBC_AllocListBit *allocator);

OBC_ERROR_ENUM OBC_AllocListBitFree(void *allocator, OBC_Offset index);
OBC_ERROR_ENUM OBC_AllocListBitFreeRaw(OBC_AllocListBit *allocator, OBC_Offset index);




/*************************************
Internal Utilities
*************************************/

#define _OBC_ALLOCLISTBIT_PTR_CAST(rawPtr) ((OBC_AllocListBit *)(rawPtr))
#define _OBC_ALLOCLISTBIT_OFFSET ((size_t)(&((OBC_AllocListBit *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCLISTBIT(rawPtr) (_OBC_ALLOCLISTBIT_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCLISTBIT_OFFSET))

#define OBC_ALLOCLISTBIT_ADDR OBC_Offset

#define OBC_ALLOCLISTBIT_META_FULL ((OBC_ALLOCLISTBIT_META)(~((OBC_ALLOCLISTBIT_META)0)))

#define OBC_FROM_ALLOCLISTBIT_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_ALLOCLISTBIT_OFFSET))
#define OBC_FROM_ALLOCLISTBIT_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_ALLOCLISTBIT_OFFSET))



void OBC_AllocListBitFlushToMem(OBC_AllocListBit *allocator);

OBC_ERROR_ENUM OBC_AllocListBitCacheExpand(void *allocator);
OBC_ERROR_ENUM OBC_AllocListBitExpandRaw(OBC_AllocListBit *allocator);

OBC_Offset OBC_AllocListBitGetNextFree(OBC_AllocListBit *allocator);
OBC_Offset OBC_AllocListBitBitPos(OBC_ALLOCLISTBIT_META meta);

void OBC_AllocListBitSetHeadLink(OBC_AllocListBit *allocator, OBC_ALLOCLISTBIT_ADDR link);
OBC_ALLOCLISTBIT_ADDR OBC_AllocListBitGetHeadLink(OBC_AllocListBit *allocator);


#endif
