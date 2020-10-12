#include "large_ray.h"


void **OBCL_newRayComplex(size_t initialReserveCount, size_t unitSize){
    OBCL_Ray *ray = malloc(sizeof(OBCL_Ray));
    if(ray==NULL){
        return NULL;
    }

    if(OBCL_initRayComplex(ray,initialReserveCount, unitSize) == OBC_ERROR_FAILURE){
        free(ray);
        return NULL;
    }

    return NULL;//OBC_RayGetDataPointer(ray);
}

OBC_ERROR_ENUM OBCL_initRayComplex(OBCL_Ray *ray, size_t initialReserveCount, size_t unitSize){

    if(OBC_initRay(& ray->addressors, sizeof(char***)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRay(& ray->containers, sizeof(OBC_Ray)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    ray->unitSize = unitSize;

    if(initialReserveCount > 0){

        OBC_Offset max = (~((OBC_Offset)0));
        OBC_Offset maxUnits;
        if(unitSize > max){
            maxUnits = 1;
        }else{
            maxUnits = max/unitSize;
        }

        do{

            OBC_Offset addr = OBC_RayNewElementRaw(& ray->addressors);
            if(addr == OBC_NULL_INDEX){
                OBCL_freeRayData(ray);
                return OBC_ERROR_FAILURE;
            }

            OBC_Offset cont = OBC_RayNewElementRaw(& ray->containers);
            if(cont == OBC_NULL_INDEX){
                OBCL_freeRayData(ray);
                return OBC_ERROR_FAILURE;
            }

            if( OBC_initRayMore(((OBC_Ray *)ray->containers.rawData) + addr,
                                        maxUnits, unitSize) == OBC_ERROR_FAILURE){
                OBCL_freeRayData(ray);
                return OBC_ERROR_FAILURE;
            }

            ((void ***)ray->containers.rawData)[cont] =
                OBC_RayGetDataPointer( ((OBC_Ray *)ray->containers.rawData) + addr);

            if(initialReserveCount > maxUnits){
                initialReserveCount-=maxUnits;
            }else{
                initialReserveCount = 0;
            }

        }while(max > 0);

    }

    return OBC_ERROR_SUCCESS;
}

void OBCL_freeRay(void *rawPtr){

    OBCL_Ray *ray = OBCL_TO_RAY_PTR(rawPtr);

    OBCL_freeRayData(ray);

    free(ray);
}

void OBCL_freeRayData(OBCL_Ray *ray){

    OBC_RayIterator iter;

    OBC_RayForEachRaw(& ray->containers, &iter){
        OBC_freeRayData( ((OBC_Ray *)ray->containers.rawData) + iter.iter );
    }

    OBC_freeRayData(& ray->containers);
    OBC_freeRayData(& ray->addressors);

}

void ***OBCL_RayGetDataPointer(OBCL_Ray *ray){
    return OBCL_FROM_RAY_PTR(ray);
}

/*


OBC_ERROR_ENUM OBC_initRay(OBC_Ray *ray, size_t initialReserveCount, size_t unitSize){

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


OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, size_t index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    void *toRemove = ray->rawData+(index*ray->unitSize);

    if(index!=ray->curUnitLength-1){
        memmove(toRemove,toRemove+ray->unitSize,(ray->curUnitLength-(index+1))*ray->unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;

}

OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, size_t index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    void *toRemove = ray->rawData+(index*ray->unitSize);
    void *toReplace = ray->rawData+((ray->curUnitLength-1)*ray->unitSize);

    if(toReplace > toRemove){
        memmove(toRemove,toReplace,ray->unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;

}

size_t OBC_RayNewElement(void *rawPtr){

    size_t where = OBC_RayCurUnitLength(rawPtr);

    if(OBC_RayDoExpand(rawPtr) == OBC_ERROR_FAILURE){
        return OBC_NULL_INDEX;
    }

    OBC_RayPushElement(rawPtr);

    return where;
}

void OBC_RayPushElement(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    ray->curUnitLength++;
}

void OBC_RayPopElement(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    ray->curUnitLength--;
}


OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    if(ray->curUnitLength>=ray->maxUnitLength){

        OBC_ERROR_PROPAGATE(OBC_RayExpand(rawPtr));

        return OBC_ERROR_SUCCESS;
    }
    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

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

        //* /
        ray->rawData = newData;
        ray->maxUnitLength = size/ray->unitSize;
    ///}

    return OBC_ERROR_SUCCESS;

}
///https://stackoverflow.com/questions/12125308/can-realloc-fail-return-null-when-trimming
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    size_t newSize = ray->maxUnitLength*ray->unitSize/2;//<<1;
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
    ray->maxUnitLength = newSize/ray->unitSize;
    if(ray->curUnitLength > ray->maxUnitLength){
        ray->curUnitLength = ray->maxUnitLength;
    }

    return OBC_ERROR_SUCCESS;

}
OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    size_t newSize = ray->maxUnitLength*ray->unitSize;//<<1;

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
    ray->maxUnitLength = newSize/ray->unitSize;

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data){

    OBC_ERROR_PROPAGATE(OBC_RayDoExpand(rawPtr));

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    rawPtr = ray->rawData;

    size_t size = ray->unitSize;

    unsigned char *tcpy = (unsigned char *)data;
    unsigned char *unit = (unsigned char *)(ray->rawData+(ray->curUnitLength*ray->unitSize));//OBC_RayGetLast(rawPtr);
    do{
        *unit=*tcpy;
        tcpy++;
        unit++;
    }while(--size);

    return OBC_ERROR_SUCCESS;

}

void* OBC_RayGetLast(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    if(ray->curUnitLength==ray->maxUnitLength){
        return ray->rawData+((ray->curUnitLength-1)*ray->unitSize);
    }
    //unsigned char *unit = (unsigned char *)(ray->rawData+ray->curLength);
    return ray->rawData+(ray->curUnitLength*ray->unitSize);
}

size_t OBC_RayCurUnitLength(void *rawPtr){
    return OBC_TO_RAY_PTR(rawPtr)->curUnitLength;
}

size_t OBC_RayCurByteLength(void *rawPtr){
    return OBC_TO_RAY_PTR(rawPtr)->curUnitLength*OBC_TO_RAY_PTR(rawPtr)->unitSize;
}

size_t OBC_RayMaxByteLength(void *rawPtr){
    return (OBC_TO_RAY_PTR(rawPtr)->maxUnitLength)*(OBC_TO_RAY_PTR(rawPtr)->unitSize);
}

size_t OBC_RayUnitSize(void *rawPtr){
    return OBC_TO_RAY_PTR(rawPtr)->unitSize;
}

size_t OBC_RayUnitMaxLength(void *rawPtr){
    return OBC_TO_RAY_PTR(rawPtr)->maxUnitLength;
}
*/
