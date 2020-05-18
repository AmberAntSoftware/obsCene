#ifndef ALLOCFASTBIT_H_INCLUDED
#define ALLOCFASTBIT_H_INCLUDED

#include "../obc.h"
#include "../containers/ray.h"

#define _OBC_ALLOCFASTBIT_PTR_CAST(rawPtr) ((OBC_AllocFastBit *)(rawPtr))
#define _OBC_ALLOCFASTBIT_OFFSET ((size_t)(&((OBC_AllocFastBit *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCFASTBIT(rawPtr) (_OBC_ALLOCFASTBIT_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCFASTBIT_OFFSET))


#define OBC_ALLOCFASTBIT_META_CHUNKSIZE 1
#define OBC_ALLOCFASTBIT_META_CHUNKSIZE_BITS (OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES*OBC_ALLOCFASTBIT_META_BITS)
#define OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES (OBC_ALLOCFASTBIT_META_CHUNKSIZE*OBC_ALLOCFASTBIT_META_BYTES)

#define OBC_ALLOCFASTBIT_META_TYPE size_t
#define OBC_ALLOCFASTBIT_META_BYTES sizeof(OBC_ALLOCFASTBIT_META_TYPE)
#define OBC_ALLOCFASTBIT_META_BITS (OBC_ALLOCFASTBIT_META_BYTES*CHAR_BIT)

#define OBC_AllocFastBitGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocFastBitGetValue(rawPtr, index) ((*(rawPtr))[index])

#define OBC_FROM_ALLOCFASTBIT_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_ALLOCFASTBIT_OFFSET))
#define OBC_FROM_ALLOCFASTBIT_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_ALLOCFASTBIT_OFFSET))

typedef struct OBC_AllocFastBit{

    OBC_Ray backed;
    OBC_Ray meta;
    OBC_Ray available;

}OBC_AllocFastBit;


void **OBC_newAllocFastBit(size_t unitSize);
void *OBC_initAllocFastBit(OBC_AllocFastBit *allocator, size_t unitSize);
void OBC_freeAllocFastBit(void *allocator);
void OBC_freeAllocFastBitData(OBC_AllocFastBit *allocator);
void **OBC_AllocFastBitGetDataPointer(OBC_AllocFastBit *allocator);

size_t OBC_AllocFastBitMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastBitFree(void *allocator, size_t pos);

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

size_t OBC_AllocFastBitMalloc2(void *allocator);









#define _OBC_ALLOCFASTBIT2_PTR_CAST(rawPtr) ((OBC_AllocFastBit2 *)(rawPtr))
#define _OBC_ALLOCFASTBIT2_OFFSET ((size_t)(&((OBC_AllocFastBit2 *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCFASTBIT2(rawPtr) (_OBC_ALLOCFASTBIT2_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCFASTBIT2_OFFSET))

typedef struct OBC_AllocFastBit2{

    OBC_Ray backed;
    OBC_Ray meta;

    size_t freeHead;
    size_t maxHead;

}OBC_AllocFastBit2;

void **OBC_newAllocFastBit2(size_t unitSize);
void *OBC_initAllocFastBit2(OBC_AllocFastBit2 *allocator, size_t unitSize);
void OBC_freeAllocFastBit2(void *allocator);
void OBC_freeAllocFastBit2Data(void *allocator);

size_t OBC_AllocFastBit2Malloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastBit2Free(void *allocator, size_t pos);

OBC_ERROR_ENUM OBC_AllocFastBit2Expand(void *allocator);

size_t OBC_AllocFastBit2GetFreeMeta(void *allocator);


#define OBC_AllocFastBit2GetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_AllocFastBit2GetValue(rawPtr, index) ((*(rawPtr))[index])


#endif // ALLOCFASTBIT_H_INCLUDED
