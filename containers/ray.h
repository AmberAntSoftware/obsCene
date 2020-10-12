#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../obc.h"

typedef struct OBC_Ray{

    ///data owned on the heap
    char *rawData;

    ///the size of the elements stored in bytes
    ///removabe if the user is forced to pass in the underlying type size
    ///uses the top 2 bits for management, so largest size is 2 ^ (sizeof(size_t)*CHAR_BIT-2)
    size_t unitSize;
    ///the current length in units stored = (curLength/unitSize)
    OBC_Offset curUnitLength;
    ///the max length in units stored = (maxLength/unitSize)
    OBC_Offset maxUnitLength;

}OBC_Ray;

/*
typedef enum OBC_RAY_MEM_ENUM {
    OBC_RAY_MEM_STATIC = 0,
    OBC_RAY_MEM_MALLOC,
    OBC_RAY_MEM_REALLOC,
    OBC_RAY_MEM_MALLOC_REALLOC,
}OBC_RAY_MEM_ENUM;
*/

/*************************************
Initialization
*************************************/

/**
returns NULL on any error
unitSize must be <= OBC_RAY_SIZE_MASK or failure
**/
void **OBC_newRay(size_t unitSize);
void **OBC_newRayComplex(OBC_Offset initialReserveCount, size_t unitSize);
/**
returns OBC_ERROR_FAILURE on any error
unitSize must be <= OBC_RAY_SIZE_MASK or failure
**/
OBC_ERROR_ENUM OBC_initRay(OBC_Ray *ray, size_t unitSize);
OBC_ERROR_ENUM OBC_initRayMore(OBC_Ray *ray, OBC_Offset initialReserveCount, size_t unitSize);
/**
returns OBC_ERROR_FAILURE on any error or null buffer with no malloc allocation allowed
0 sized allocation is allowed to perform no-op memory counters
unitSize must be <= OBC_RAY_SIZE_MASK or failure

ray -- meta to intialize
bufferStorage -- small size optimization or static buffer
initialReserveCount -- amount to allocate or max size of buffer (0 does not malloc until rayAdd/new/emplace)
unitSize -- the size of each unit
bufferIsReallocable -- small storage optimization or buffer stored elsewhere, prevent realloc'ing bufferPointer
newBufferCanMalloc -- prevent any malloc allocations when the buffer specified is full (causes error if no buffer or 0 max init buffer is passed on init)
**/
OBC_ERROR_ENUM OBC_initRayComplex(OBC_Ray *ray, void* bufferStorage, OBC_Offset initialReserveCount, size_t unitSize, OBC_bool bufferIsReallocable, OBC_bool newBufferCanMalloc);

/*************************************
Deallocation
*************************************/

///Frees the dynamic ray allocation and the owned dynamic data owned
void OBC_freeRay(void *rawPtr);
///Frees only the owned dynamic data owned
void OBC_freeRayData(OBC_Ray *ray);

/*************************************
Data Accessors
*************************************/

void **OBC_RayGetDataPointer(OBC_Ray *ray);

///gets the number of units stored in this ray
OBC_Offset OBC_RayGetCurUnitLength(void *rawPtr);
OBC_Offset OBC_RayGetCurUnitLengthRaw(OBC_Ray *ray);
///gets the total bytes this ray is using
size_t OBC_RayGetCurByteLength(void *rawPtr);
size_t OBC_RayGetCurByteLengthRaw(OBC_Ray *ray);
///gets the total allocated bytes
size_t OBC_RayGetMaxByteLength(void *rawPtr);
size_t OBC_RayGetMaxByteLengthRaw(OBC_Ray *ray);
///gets the size in bytes of each element stored
OBC_Offset OBC_RayGetUnitSize(void *rawPtr);
OBC_Offset OBC_RayGetUnitSizeRaw(OBC_Ray *ray);
///gets the total allocated units
OBC_Offset OBC_RayGetMaxUnitLength(void *rawPtr);
OBC_Offset OBC_RayGetMaxUnitLengthRaw(OBC_Ray *ray);

/**
Determines if the buffer is able to grow with realloc
No realloc, buffer is small optimization or owned in other data
*/
OBC_bool OBC_RayIsReallocable(void *rawPtr);
OBC_bool OBC_RayIsReallocableRaw(OBC_Ray *ray);
/**
Determines if the buffer is able to use malloc
No malloc, prevents all reallocation and can only use what storage is passed
*/
OBC_bool OBC_RayIsMallocable(void *rawPtr);
OBC_bool OBC_RayIsMallocableRaw(OBC_Ray *ray);

/*************************************
Operations
*************************************/

///expands if necessary
OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr);
OBC_ERROR_ENUM OBC_RayDoExpandRaw(OBC_Ray *ray);
///actually expand size*2, regardless if storage limit has been reached
OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr);
OBC_ERROR_ENUM OBC_RayExpandRaw(OBC_Ray *ray);
///actually contract size/2, minimum heap size is 1 unitSize
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr);
OBC_ERROR_ENUM OBC_RayContractRaw(OBC_Ray *ray);
///fits the size of the ray to the maximum of currently contained objects
OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr);
OBC_ERROR_ENUM OBC_RayShrinkToFitRaw(OBC_Ray *ray);

///sets the last position to the next element and attempts expansion if full
OBC_Offset OBC_RayNewElement(void *rawPtr);
OBC_Offset OBC_RayNewElementRaw(OBC_Ray *ray);
///sets the last position to the next element
void OBC_RayPushElement(void *rawPtr);
void OBC_RayPushElementRaw(OBC_Ray *ray);
///sets the last position to the previous element
void OBC_RayPopElement(void *rawPtr);
void OBC_RayPopElementRaw(OBC_Ray *ray);

///sets the value in the last element in the ray
OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data);
OBC_ERROR_ENUM OBC_RaySetLastRaw(OBC_Ray *ray, void *data);

///removes, frees and pushes all elements back in order into the empty element
OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, OBC_Offset index);
OBC_ERROR_ENUM OBC_RayRemoveRaw(OBC_Ray *ray, OBC_Offset index);
///removes, frees and places the last element into the removed element's location
OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, OBC_Offset index);
OBC_ERROR_ENUM OBC_RayRemoveFastRaw(OBC_Ray *ray, OBC_Offset index);
///returns the last index address
void* OBC_RayGetLast(void *rawPtr);
void* OBC_RayGetLastRaw(OBC_Ray *ray);


/*************************************
Iteration
*************************************/

typedef struct OBC_RayIterator{
    OBC_Offset iter;
    OBC_Offset endIter;
}OBC_RayIterator;

#define OBC_RayForEach(arrPtr, iterPtr) \
for(OBC_RayIterStart(arrPtr, iterPtr); (iterPtr)->iter < (iterPtr)->endIter; OBC_RayIterNext(arrPtr, iterPtr))

#define OBC_RayForEachRaw(rayPtr, iterPtr) \
for(OBC_RayIterStartRaw(rayPtr, iterPtr); (iterPtr)->iter < (iterPtr)->endIter; OBC_RayIterNextRaw(rayPtr, iterPtr))


#define OBC_RayForEachSub(arrPtr, iterPtr, startOffset, endOffset) \
OBC_RayIterStart(arrPtr, iterPtr); \
(iterPtr)->iter += startOffset; \
(iterPtr)->endIter -= endOffset; \
for(; (iterPtr)->iter < (iterPtr)->endIter; OBC_RayIterNext(arrPtr, iterPtr))

void OBC_RayIterStart(void *rawPtr, OBC_RayIterator *iter);
void OBC_RayIterStartRaw(OBC_Ray *ray, OBC_RayIterator *iter);
void OBC_RayIterNext(void *rawPtr, OBC_RayIterator *iter);
void OBC_RayIterNextRaw(OBC_Ray *ray, OBC_RayIterator *iter);

/*************************************
Alternate Operations
*************************************/
/*
https://en.cppreference.com/w/cpp/container/vector
https://en.cppreference.com/w/cpp/container/vector/data
*/
#define OBC_RayAddValue(rawPtr, dataVaue)\
    if(OBC_TO_RAY_PTR(rawPtr)->curUnitLength==OBC_TO_RAY_PTR(rawPtr)->maxUnitLength){\
        if(OBC_RayExpand(rawPtr) != OBC_ERROR_FAILURE){\
            (*((*(rawPtr))+OBC_TO_RAY_PTR(rawPtr)->curUnitLength)) = dataVaue;\
            OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
        }\
    }else{\
        (*((*(rawPtr))+OBC_TO_RAY_PTR(rawPtr)->curUnitLength)) = dataVaue;\
        OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
    }
#define OBC_RayAddPointer(rawPtr, dataPtr) do{ if(OBC_RayDoExpand(rawPtr)!= OBC_ERROR_FAILURE){ OBC_RaySetLast(rawPtr, dataPtr); OBC_RayNewElement(rawPtr); } }while(0);


#define OBC_RayNewElementFast(size_tStorage,rawPtr)\
    do{\
    if(OBC_TO_RAY_PTR(rawPtr)->curUnitLength==OBC_TO_RAY_PTR(rawPtr)->maxUnitLength){\
            if(OBC_RayExpand(rawPtr) != OBC_ERROR_FAILURE){\
                size_tStorage = OBC_TO_RAY_PTR(rawPtr)->curUnitLength;\
                OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
            }else{\
                size_tStorage = OBC_NULL_INDEX;\
            }\
    }else{\
        size_tStorage = OBC_TO_RAY_PTR(rawPtr)->curUnitLength;\
        OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
    }\
    }while(0);



/*************************************
Internal Utilities
*************************************/

#define OBC_RAY_SIZE_MASK (((~((size_t)0))-7)>>3)
#define OBC_RAY_CAN_REALLOC_MASK (((size_t)1)<<(CHAR_BIT*sizeof(size_t)-1))
#define OBC_RAY_CAN_MALLOC_MASK  (((size_t)1)<<(CHAR_BIT*sizeof(size_t)-2))
#define OBC_RAY_CAN_FREE_MASK  (((size_t)1)<<(CHAR_BIT*sizeof(size_t)-2))

#define _OBC_RAY_PTR_CAST(rawPtr) ((OBC_Ray *)(rawPtr))
#define _OBC_RAY_OFFSET ((size_t)(&((OBC_Ray *)NULL)->rawData))
#define OBC_TO_RAY_PTR(rawPtr) (_OBC_RAY_PTR_CAST(((void*)(rawPtr)) - _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_RAY_OFFSET))


#endif // RAY_H_INCLUDED
