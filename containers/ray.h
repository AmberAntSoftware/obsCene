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
    size_t unitSize;
    ///the current length in units stored = (curLength/unitSize)
    OBC_Offset curUnitLength;
    ///the max length in units stored = (maxLength/unitSize)
    OBC_Offset maxUnitLength;

}OBC_Ray;

/*************************************
Initialization
*************************************/

///NULL on failed allocation
void **OBC_newRay(size_t unitSize);
void **OBC_newRayComplex(OBC_Offset initialReserveCount, size_t unitSize);
///FAIL on error
OBC_ERROR_ENUM OBC_initRay(OBC_Ray *ray, size_t unitSize);
OBC_ERROR_ENUM OBC_initRayComplex(OBC_Ray *ray, OBC_Offset initialReserveCount, size_t unitSize);

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
OBC_Offset OBC_Ray_CurUnitLength(void *rawPtr);
OBC_Offset OBC_Ray_CurUnitLengthRaw(OBC_Ray *ray);
///gets the total bytes this ray is using
size_t OBC_Ray_CurByteLength(void *rawPtr);
size_t OBC_Ray_CurByteLengthRaw(OBC_Ray *ray);
///gets the total allocated bytes
size_t OBC_Ray_MaxByteLength(void *rawPtr);
size_t OBC_Ray_MaxByteLengthRaw(OBC_Ray *ray);
///gets the size in bytes of each element stored
OBC_Offset OBC_Ray_UnitSize(void *rawPtr);
OBC_Offset OBC_Ray_UnitSizeRaw(OBC_Ray *ray);
///gets the total allocated units
OBC_Offset OBC_Ray_MaxUnitLength(void *rawPtr);
OBC_Offset OBC_Ray_MaxUnitLengthRaw(OBC_Ray *ray);

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

#define OBC_ERROR_PROPAGATE(tokenResolvingToRayEnumValue) do { if( (tokenResolvingToRayEnumValue) == OBC_ERROR_FAILURE ) { return OBC_ERROR_FAILURE; } } while(0);

#define _OBC_RAY_PTR_CAST(rawPtr) ((OBC_Ray *)(rawPtr))
#define _OBC_RAY_OFFSET ((size_t)(&((OBC_Ray *)NULL)->rawData))
#define OBC_TO_RAY_PTR(rawPtr) (_OBC_RAY_PTR_CAST(((void*)(rawPtr)) - _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_RAY_OFFSET))


#endif // RAY_H_INCLUDED
