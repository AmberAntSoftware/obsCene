#ifndef LARGE_RAY_H_INCLUDED
#define LARGE_RAY_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../obc.h"
#include "../../containers/ray.h"

//*

#define _OBCL_RAY_PTR_CAST(rawPtr) ((OBCL_Ray *)(rawPtr))
#define _OBCL_RAY_OFFSET ((size_t)(&((OBCL_Ray *)NULL)->addressors.rawData))
#define OBCL_TO_RAY_PTR(rawPtr) (_OBCL_RAY_PTR_CAST(((void*)(rawPtr)) - _OBCL_RAY_OFFSET))
#define OBCL_FROM_RAY_PTR(rawPtr) ((void***)(((void*)(rawPtr)) + _OBCL_RAY_OFFSET))
#define OBCL_FROM_RAY_VAL(rayVal) ((void***)(((void*)(&(rayVal))) + _OBCL_RAY_OFFSET))
//*/

typedef struct OBCL_Ray{

    OBC_Ray addressors;
    OBC_Ray containers;

    size_t unitSize;
    ///the current length in Ray units stored
    OBC_Offset curLength;
    ///the max length in Ray units stored
    OBC_Offset maxLength;

}OBCL_Ray;


///NULL on failed allocation
void **OBCL_newRayComplex(size_t initialReserveCount, size_t unitSize);
///NULL on error
OBC_ERROR_ENUM OBCL_initRayComplex(OBCL_Ray *ray, size_t initialReserveCount, size_t unitSize);

void OBCL_freeRay(void *rawPtr);
void OBCL_freeRayData(OBCL_Ray *ray);

void ***OBCL_RayGetDataPointer(OBCL_Ray *ray);

#endif // LARGE_RAY_H_INCLUDED
