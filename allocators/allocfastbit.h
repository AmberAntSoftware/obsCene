#ifndef ALLOCFASTBIT_H_INCLUDED
#define ALLOCFASTBIT_H_INCLUDED

#include "OBC.h"
#include "ray.h"

#define _OBC_ALLOCFASTBIT_PTR_CAST(rawPtr) ((OBC_AllocFastBit *)(rawPtr))
#define _OBC_ALLOCFASTBIT_OFFSET ((size_t)(&((OBC_AllocFastBit *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCFASTBIT(rawPtr) (_OBC_ALLOCFASTBIT_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCFASTBIT_OFFSET))



#define OBC_ALLOCFASTBIT_META_TYPE size_t
#define OBC_ALLOCFASTBIT_META_BITS (sizeof(OBC_ALLOCFASTBIT_META_TYPE)*CHAR_BIT)

typedef struct OBC_AllocFastBit{

    OBC_Ray backed;
    OBC_Ray meta;
    OBC_Ray available;

}OBC_AllocFastBit;


void **OBC_newAllocFastBit(size_t unitSize);
void *OBC_initAllocFastBit(OBC_AllocFastBit *allocator, size_t unitSize);
void OBC_freeAllocFastBit(void *allocator);
void OBC_freeAllocFastBitData(void *allocator);

size_t OBC_AllocFastBitMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastFree(void *allocator, size_t data);

size_t OBC_AllocFastBitGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastBitExpand(void *allocator);

///UTILITY
///utility -- marks the location bit as free
void OBC_AllocFastBitMarkFreed(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as used
void OBC_AllocFastBitMarkAllocated(void *allocator, size_t pos);


OBC_ERROR_ENUM OBC_AllocFastBitBalanceMeta(void *allocator);

size_t OBC_AllocFastBitGetAvailable(void *allocator);
size_t OBC_AllocFastBitGetFirstBit(OBC_ALLOCFASTBIT_META_TYPE val);


#define OBC_AllocFastBitGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocFastBitGetValue(rawPtr, index) ((*(rawPtr))[index])


#endif // ALLOCFASTBIT_H_INCLUDED
