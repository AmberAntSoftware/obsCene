#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include <string.h>

#include "OBC.h"

#define OBC_ERROR_PROPAGATE(tokenResolvingToRayEnumValue) do { if( (tokenResolvingToRayEnumValue) == OBC_ERROR_FAILURE ) { return OBC_ERROR_FAILURE; } } while(0);


#define _OBC_RAY_PTR_CAST(rawPtr) ((OBC_Ray *)(rawPtr))
#define _OBC_RAY_OFFSET ((size_t)(&((OBC_Ray *)NULL)->rawData))
#define OBC_TO_RAY_PTR(rawPtr) (_OBC_RAY_PTR_CAST(((void*)(rawPtr)) - _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_RAY_OFFSET))
#define OBC_FROM_RAY_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_RAY_OFFSET))

typedef struct OBC_Ray{

    ///data owned on the heap
    char *rawData;

    ///current length in bytes
    size_t curLength;
    ///max allocated size in bytes
    size_t maxLength;
    ///the size of the elements stored in bytes
    size_t unitSize;
    ///the current length in units stored = (curLength/unitSize)
    size_t curUnitLength;
    ///the max length in units stored = (maxLength/unitSize)
    size_t maxUnitLength;

}OBC_Ray;


///NULL on failed allocation
void **OBC_newRay(size_t initialReserveCount, size_t unitSize);
///NULL on error
void *OBC_initRay(OBC_Ray *ray, size_t initialReserveCount, size_t unitSize);
///Frees the heap ray allocation and the owned data
void OBC_freeRay(void *rawPtr);
///Frees only the stack data owned by the ray
void OBC_freeRayData(OBC_Ray *ray);

void **OBC_RayGetDataPointer(OBC_Ray *ray);

/***************************************
gets the values associated with the dealigned ray
***************************************/
///gets the number of units stored in this ray
size_t OBC_RayCurUnitLength(void *rawPtr);
///gets the total bytes this ray is using
size_t OBC_RayLength(void *rawPtr);
///gets the total allocated bytes
size_t OBC_RayMaxLength(void *rawPtr);
///gets the size in bytes of each element stored
size_t OBC_RayUnitSize(void *rawPtr);
///gets the total allocated units
size_t OBC_RayUnitMaxLength(void *rawPtr);

#define OBC_RayGetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_RayGetValue(rawPtr, index) ((*(rawPtr))[index])


/*
https://en.cppreference.com/w/cpp/container/vector
https://en.cppreference.com/w/cpp/container/vector/data
*/
/*
#define OBC_RayAddValue(rawPtr, data) do{ if(OBC_RayDoExpand(rawPtr)!= OBC_FAILURE){ (*((*(rawPtr))+OBC_TO_RAW_RAY(rawPtr)->unitLength)) = data; OBC_RayNewElement(rawPtr); } }while(0);
/*/
    //**
    #define OBC_RayAddValue(rawPtr, data)\
        if(OBC_TO_RAY_PTR(rawPtr)->curLength==OBC_TO_RAY_PTR(rawPtr)->maxLength){\
            if(OBC_RayExpand(rawPtr) != OBC_ERROR_FAILURE){\
                (*((*(rawPtr))+OBC_TO_RAY_PTR(rawPtr)->curUnitLength)) = data;\
                OBC_TO_RAY_PTR(rawPtr)->curLength+=OBC_TO_RAY_PTR(rawPtr)->unitSize;\
                OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
            }\
        }else{\
            (*((*(rawPtr))+OBC_TO_RAY_PTR(rawPtr)->curUnitLength)) = data;\
            OBC_TO_RAY_PTR(rawPtr)->curLength+=OBC_TO_RAY_PTR(rawPtr)->unitSize;\
            OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
        }
    //**/
//*/
//#define OBC_RayAddValue(rawPtr, data) do{ if(OBC_RayDoExpand(rawPtr)!= OBC_FAILURE){ (*((*(rawPtr))+OBC_TO_RAW_RAY(rawPtr)->unitLength)) = data; OBC_RayPushElement(rawPtr); } }while(0);


#define OBC_RayAddPointer(rawPtr, dataPtr) do{ if(OBC_RayDoExpand(rawPtr)!= OBC_ERROR_FAILURE){ OBC_RaySetLast(rawPtr, dataPtr); OBC_RayNewElement(rawPtr); } }while(0);

/*#define OBC_RayGetNewPointer(rawPtr)
#define OBC_RayGetNewValue(rawPtr)
#define OBC_RayFirstValue(rawPtr)
#define OBC_RayLastValue(rawPtr)
#define OBC_RayFirstPtr(rawPtr)
#define OBC_RayLastPtr(rawPtr)*/
//#define OBC_RayLoopHead(rawPtr, counterNameSizeT) size_t counterNameSizeT = 0; for(counterNameSizeT < )

//#define OBC_Ray

///expands if necessary
OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr);
///actually expand size*2, regardless if storage limit has been reached
OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr);
///actually contract size/2, minimum heap size is 1 unitSize
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr);
///fits the size of the ray to the maximum of currently contained objects
OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr);

///sets the last position to the next element and attempts expansion if full
size_t OBC_RayNewElement(void *rawPtr);
///sets the last position to the next element
void OBC_RayPushElement(void *rawPtr);
///sets the last position to the previous element
void OBC_RayPopElement(void *rawPtr);

///sets the value in the last element in the ray
OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data);

///removes, frees and pushes all elements back in order into the empty element
OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, size_t index);
///removes, frees and places the last element into the removed element's location
OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, size_t index);
///returns the last index address
void* OBC_RayGetLast(void *rawPtr);

#define OBC_RayNewElementFast(size_tStorage,rawPtr)\
    do{\
    if(OBC_TO_RAY_PTR(rawPtr)->curLength==OBC_TO_RAY_PTR(rawPtr)->maxLength){\
            if(OBC_RayExpand(rawPtr) != OBC_ERROR_FAILURE){\
                size_tStorage = OBC_TO_RAY_PTR(rawPtr)->curUnitLength;\
                OBC_TO_RAY_PTR(rawPtr)->curLength+=OBC_TO_RAY_PTR(rawPtr)->unitSize;\
                OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
            }else{\
                size_tStorage = OBC_NULL_INDEX;\
            }\
    }else{\
        size_tStorage = OBC_TO_RAY_PTR(rawPtr)->curUnitLength;\
        OBC_TO_RAY_PTR(rawPtr)->curLength+=OBC_TO_RAY_PTR(rawPtr)->unitSize;\
        OBC_TO_RAY_PTR(rawPtr)->curUnitLength++;\
    }\
    }while(0);

#endif // RAY_H_INCLUDED
