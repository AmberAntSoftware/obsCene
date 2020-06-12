#include "ray.h"


void **OBC_newRay(size_t unitSize){
    return OBC_newRayComplex(0, unitSize);
}

void **OBC_newRayComplex(OBC_Offset initialReserveCount, size_t unitSize){
    OBC_Ray *ray = malloc(sizeof(OBC_Ray));
    if(ray==NULL){
        return NULL;
    }

    if(OBC_initRayComplex(ray,initialReserveCount, unitSize) == OBC_ERROR_FAILURE){
        free(ray);
        return NULL;
    }

    return OBC_RayGetDataPointer(ray);
}

OBC_ERROR_ENUM OBC_initRay(OBC_Ray *ray, size_t unitSize){
    return OBC_initRayComplex(ray, 0, unitSize);
}

OBC_ERROR_ENUM OBC_initRayComplex(OBC_Ray *ray, OBC_Offset initialReserveCount, size_t unitSize){

    if(initialReserveCount==0 || unitSize == 0 || initialReserveCount == 0){
        ray->rawData=NULL;
        ray->maxUnitLength = 0;
    }else{
        ray->rawData = malloc(unitSize*initialReserveCount);
        if(ray->rawData==NULL){
            return OBC_ERROR_FAILURE;
        }
        ray->maxUnitLength = initialReserveCount;
    }

    ray->curUnitLength = 0;
    ray->unitSize = unitSize;

    return OBC_ERROR_SUCCESS;

}

void OBC_freeRay(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_freeRayData(ray);
    free(ray);
}

void OBC_freeRayData(OBC_Ray *ray){
    free(ray->rawData);
}

void **OBC_RayGetDataPointer(OBC_Ray *ray){
    return OBC_FROM_RAY_PTR(ray);
}


OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, OBC_Offset index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayRemoveRaw(ray, index);
}

OBC_ERROR_ENUM OBC_RayRemoveRaw(OBC_Ray *ray, OBC_Offset index){

    void *toRemove = ray->rawData+(index*ray->unitSize);

    if(index!=ray->curUnitLength-1){
        memmove(toRemove,toRemove+ray->unitSize,(ray->curUnitLength-(index+1))*ray->unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;
}


OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, OBC_Offset index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    return OBC_RayRemoveFastRaw(ray, index);
}

OBC_ERROR_ENUM OBC_RayRemoveFastRaw(OBC_Ray *ray, OBC_Offset index){

    void *toRemove = ray->rawData+(index*ray->unitSize);
    void *toReplace = ray->rawData+((ray->curUnitLength-1)*ray->unitSize);
    /*if(index!=ray->unitLength-1 && toReplace > toRemove){*/
    if(toReplace > toRemove){
        memmove(toRemove,toReplace,ray->unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;
}


OBC_Offset OBC_RayNewElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayNewElementRaw(ray);
}

OBC_Offset OBC_RayNewElementRaw(OBC_Ray *ray){

    OBC_Offset where = ray->curUnitLength;
    if(OBC_RayDoExpandRaw(ray) == OBC_ERROR_FAILURE){
        return OBC_NULL_INDEX;
    }

    OBC_RayPushElementRaw(ray);
    return where;
}

void OBC_RayPushElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_RayPushElementRaw(ray);
}

void OBC_RayPushElementRaw(OBC_Ray *ray){
    ray->curUnitLength++;
}


void OBC_RayPopElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_RayPopElementRaw(ray);
}

void OBC_RayPopElementRaw(OBC_Ray *ray){
    ray->curUnitLength--;
}



OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayDoExpandRaw(ray);
}

OBC_ERROR_ENUM OBC_RayDoExpandRaw(OBC_Ray *ray){

    if(ray->curUnitLength>=ray->maxUnitLength){
        OBC_ERROR_PROPAGATE(OBC_RayExpandRaw(ray));
        return OBC_ERROR_SUCCESS;
    }
    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayExpandRaw(ray);
}

OBC_ERROR_ENUM OBC_RayExpandRaw(OBC_Ray *ray){
    if(ray->unitSize == 0){
        return OBC_ERROR_NO_OP;
    }

    size_t size = ray->maxUnitLength*ray->unitSize*2;//<<1;
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
        //** from 0 allocaions

        //*/
        ray->rawData = newData;
        ray->maxUnitLength = (OBC_Offset)(size/ray->unitSize);
    ///}

    return OBC_ERROR_SUCCESS;
}


///https://stackoverflow.com/questions/12125308/can-realloc-fail-return-null-when-trimming
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayContractRaw(ray);
}

OBC_ERROR_ENUM OBC_RayContractRaw(OBC_Ray *ray){
    if(ray->unitSize == 0){
        return OBC_ERROR_NO_OP;
    }

    size_t newSize = ((size_t)ray->maxUnitLength)*ray->unitSize/2;//<<1;
    if(newSize < ray->unitSize){
        /*
        newSize = 0;
        OBC_freeRayData(ray);
        ray->rawData = NULL;
        ray->curUnitLength = 0;
        ray->maxUnitLength = 0;
        return;
        /*/
        ray->unitSize = ray->unitSize;
        //*/
    }

    char *newData;
    if(ray->rawData==NULL || ray->unitSize == 0){
        newData = malloc(newSize);
    }else{
        newData = realloc(ray->rawData,newSize);
    }

    if(newData == NULL){
        return OBC_ERROR_FAILURE;
    }
    ray->rawData = newData;
    ray->maxUnitLength = (OBC_Offset)(newSize/ray->unitSize);
    if(ray->curUnitLength > ray->maxUnitLength){
        ray->curUnitLength = ray->maxUnitLength;
    }

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayShrinkToFitRaw(ray);
}

OBC_ERROR_ENUM OBC_RayShrinkToFitRaw(OBC_Ray *ray){

    size_t newSize = ((size_t)ray->maxUnitLength)*ray->unitSize;//<<1;
    char *newData;
    if(ray->rawData==NULL || ray->unitSize == 0){
        return OBC_ERROR_NO_OP;
    }else{
        newData = realloc(ray->rawData,newSize);
    }

    if(newData==NULL){
        return OBC_ERROR_FAILURE;
    }

    ray->rawData = newData;
    ray->maxUnitLength = (OBC_Offset)(newSize/ray->unitSize);

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RaySetLastRaw(ray, data);
}

OBC_ERROR_ENUM OBC_RaySetLastRaw(OBC_Ray *ray, void *data){

    OBC_ERROR_PROPAGATE(OBC_RayDoExpandRaw(ray));

    size_t size = ray->unitSize;
    unsigned char *tcpy = (unsigned char *)data;
    unsigned char *unit = (unsigned char *)(ray->rawData+(ray->curUnitLength*ray->unitSize));
    do{
        *unit=*tcpy;
        tcpy++;
        unit++;
    }while(--size);

    return OBC_ERROR_SUCCESS;
}

void* OBC_RayGetLast(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetLastRaw(ray);
}

void* OBC_RayGetLastRaw(OBC_Ray *ray){

    if(ray->curUnitLength == ray->maxUnitLength){
        return ray->rawData +(((size_t)(ray->curUnitLength-1))*ray->unitSize);
    }
    //unsigned char *unit = (unsigned char *)(ray->rawData+ray->curLength);
    return ray->rawData+(((size_t)ray->curUnitLength)*ray->unitSize);
}

OBC_Offset OBC_Ray_CurUnitLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_Ray_CurUnitLengthRaw(ray);
}

OBC_Offset OBC_Ray_CurUnitLengthRaw(OBC_Ray *ray){
    return ray->curUnitLength;
}

size_t OBC_Ray_CurByteLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_Ray_CurByteLengthRaw(ray);
}

size_t OBC_Ray_CurByteLengthRaw(OBC_Ray *ray){
    return ((size_t)ray->curUnitLength) * ray->unitSize;
}

size_t OBC_Ray_MaxByteLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_Ray_MaxByteLengthRaw(ray);
}

size_t OBC_Ray_MaxByteLengthRaw(OBC_Ray *ray){
    return ((size_t)ray->maxUnitLength) * ray->unitSize;
}

OBC_Offset OBC_Ray_UnitSize(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_Ray_UnitSizeRaw(ray);
}

OBC_Offset OBC_Ray_UnitSizeRaw(OBC_Ray *ray){
    return ray->unitSize;
}

OBC_Offset OBC_Ray_MaxUnitLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_Ray_MaxUnitLengthRaw(ray);
}

OBC_Offset OBC_Ray_MaxUnitLengthRaw(OBC_Ray *ray){
    return ray->maxUnitLength;
}

void OBC_RayIterStart(void *arr, OBC_RayIterator *iter){
    OBC_RayIterStartRaw(OBC_TO_RAY_PTR(arr),iter);
}
void OBC_RayIterStartRaw(OBC_Ray *ray, OBC_RayIterator *iter){
    iter->iter = 0;
    iter->endIter = OBC_Ray_CurUnitLengthRaw(ray);
}
void OBC_RayIterNext(void *arr, OBC_RayIterator *iter){
    OBC_RayIterNextRaw(OBC_TO_RAY_PTR(arr),iter);
}
void OBC_RayIterNextRaw(OBC_Ray *ray, OBC_RayIterator *iter){
    iter->iter++;
}
