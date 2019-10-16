#include "ray.h"

#include <stdlib.h>
#include <stdio.h>

void **OBC_newRay(size_t initialReserveCount, size_t unitSize){
    OBC_Ray *ray = malloc(sizeof(OBC_Ray));
    if(ray==NULL){
        return NULL;
    }

    if(OBC_initRay(ray,initialReserveCount, unitSize) == NULL){
        free(ray);
        return NULL;
    }

    return (void**)&(ray->rawData);
}

void *OBC_initRay(OBC_Ray *ray, size_t initialReserveCount, size_t unitSize){

    if(initialReserveCount==0){
        ray->rawData=NULL;
    }else{
        ray->rawData = malloc(unitSize*initialReserveCount);
        if(ray->rawData==NULL){
            return NULL;
        }
    }

    ray->curLength = 0;
    ray->curUnitLength = 0;
    ray->unitSize = unitSize;
    ray->maxLength = unitSize*initialReserveCount;
    ray->maxUnitLength = ray->maxLength*ray->unitSize;

    return ray;

}

void OBC_freeRay(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    OBC_freeRayData(rawPtr);
    free(ray);
}

void OBC_freeRayData(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    OBC_freeRayData(ray->rawData);
}


OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, size_t index){

    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);

    void *toRemove = ray->rawData+(index*ray->unitSize);

    if(index!=ray->curUnitLength-1){
        memmove(toRemove,toRemove+ray->unitSize,(ray->curUnitLength-(index+1))*ray->unitSize);
    }

    ray->curLength-=ray->unitSize;
    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;

}

OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, size_t index){

    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);

    void *toRemove = ray->rawData+(index*ray->unitSize);
    void *toReplace = ray->rawData+ray->curLength-ray->unitSize;
    /*if(index!=ray->unitLength-1 && toReplace > toRemove){*/
    if(toReplace > toRemove){
        memmove(toRemove,toReplace,ray->unitSize);
    }

    ray->curLength-=ray->unitSize;
    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;

}

OBC_ERROR_ENUM OBC_RayNewElement(void *rawPtr){

    OBC_RayPushElement(rawPtr);

    OBC_RAY_ERROR_PROPAGATE(OBC_RayDoExpand(rawPtr));

    return OBC_ERROR_SUCCESS;
}

void OBC_RayPushElement(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);

    ray->curLength+=ray->unitSize;
    ray->curUnitLength++;
}

void OBC_RayPopElement(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);

    ray->curLength-=ray->unitSize;
    ray->curUnitLength--;
}


OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    if(ray->curLength==ray->maxLength){

        OBC_RAY_ERROR_PROPAGATE(OBC_RayExpand(rawPtr));

        return OBC_ERROR_SUCCESS;
    }
    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    size_t size = ray->maxLength*2;//<<1;
    if(size==0){
        size=ray->unitSize;
    }

    ///if(ray->curLength == ray->maxLength){
        char *newData;
        if(ray->rawData==NULL){
            newData = malloc(size);
        }else{
            newData = realloc(ray->rawData,size);
        }

        if(newData == NULL){
            return OBC_ERROR_FAILURE;
        }
        ray->rawData = newData;
        ray->maxLength = size;
        ray->maxUnitLength = size*ray->unitSize;
    ///}

    return OBC_ERROR_SUCCESS;

}
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    size_t newSize = ray->maxLength/2;//<<1;
    if(newSize < ray->unitSize){
        newSize=ray->unitSize;
    }

    char *newData;
    if(ray->rawData==NULL){
        newData = malloc(newSize);
    }else{
        newData = realloc(ray->rawData,newSize);
    }

    if(newData == NULL){
        return OBC_ERROR_FAILURE;
    }
    ray->rawData = newData;
    ray->maxLength = newSize;
    ray->maxUnitLength = newSize*ray->unitSize;
    if(ray->curLength > ray->maxLength){
        ray->curLength = ray->maxLength;
        ray->curUnitLength = ray->maxUnitLength;
    }

    return OBC_ERROR_SUCCESS;

}
OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    size_t newSize = ray->curLength;//<<1;

    char *newData;
    if(ray->rawData==NULL){
        return OBC_ERROR_NO_OP;
    }else{
        newData = realloc(ray->rawData,newSize);
    }

    if(newData==NULL){
        return OBC_ERROR_FAILURE;
    }

    ray->rawData = newData;
    ray->maxLength = newSize;
    ray->maxUnitLength = ray->maxLength*ray->unitSize;

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data){

    OBC_RAY_ERROR_PROPAGATE(OBC_RayDoExpand(rawPtr));

    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    rawPtr = ray->rawData;

    size_t size = ray->unitSize;

    char *tcpy = (char *)data;
    char *unit = (char *)OBC_RayGetLast(rawPtr);
    do{
        *unit=*tcpy;
        tcpy++;
        unit++;
    }while(--size);

    return OBC_ERROR_SUCCESS;

}

void* OBC_RayGetLast(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAW_RAY(rawPtr);
    if(ray->curLength==ray->maxLength){
        return ray->rawData+ray->curLength-ray->unitSize;
    }
    return ray->rawData+ray->curLength;
}

size_t OBC_RayCurUnitLength(void *rawPtr){
    return OBC_TO_RAW_RAY(rawPtr)->curUnitLength;
}

size_t OBC_RayLength(void *rawPtr){
    return OBC_TO_RAW_RAY(rawPtr)->curLength;
}

size_t OBC_RayMaxLength(void *rawPtr){
    return OBC_TO_RAW_RAY(rawPtr)->maxLength;
}

size_t OBC_RayUnitSize(void *rawPtr){
    return OBC_TO_RAW_RAY(rawPtr)->unitSize;
}

size_t OBC_RayUnitMaxLength(void *rawPtr){
    return OBC_TO_RAW_RAY(rawPtr)->maxUnitLength;
}