#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"

//static OBC_AllocRay hiddenAlloc;

void **OBC_newAllocRay(size_t unitSize, void (*freeFunction)(void *value)){

    OBC_AllocRay *allocator = calloc(1,sizeof(OBC_AllocRay));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocRay(allocator, unitSize, freeFunction) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

void *OBC_initAllocRay(void *allocator, size_t unitSize, void (*freeFunction)(void *value)){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    if(OBC_initRay(&allocRay->backed,sizeof(OBC_ALLOCATOR_META_TYPE),unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(&allocRay->meta,1,sizeof(OBC_ALLOCATOR_META_TYPE)) == NULL){
        OBC_freeRayData(&allocRay->backed);
        return NULL;
    }

    return allocRay;

}

size_t OBC_AllocRayMalloc(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    size_t place = OBC_AllocRayGetFreeLocation(allocator);
    if(place == OBC_NULL_INDEX){
        if(OBC_AllocRayExpand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
        place = allocRay->backed.curUnitLength;
    }

    OBC_AllocRayMarkAllocated(allocator,place);

    return place;
}

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    if(allocRay->freeFunction != NULL){
        void *ptr = allocRay->backed.rawData + (allocRay->backed.unitSize*data);
        allocRay->freeFunction(ptr);
    }

    OBC_AllocRayMarkFreed(allocator,data);

    return OBC_ERROR_NO_OP;

}

size_t OBC_AllocRayGetFreeLocation(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

///FIXME based on meta length, not unit length
    size_t max = allocRay->meta.curUnitLength;
    size_t i;
    size_t place = OBC_NULL_INDEX;

    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    for(i = 0; i < max; i++){
        if(raw[i]!=~0){
            break;
        }
    }

    if(i!=max){
        place = (i*sizeof(OBC_ALLOCATOR_META_TYPE));
        place += OBC_FindFirstEmptyBit(raw[i]);
        if(allocRay->backed.unitSize*place >= allocRay->backed.maxLength){
            place = OBC_NULL_INDEX;
        }
    }

    /*
    if(place >= allocRay->backed.unitLength || i == max){
        OBC_AllocRayExpand(allocator);
    }else{
        return place;
    }*/

    return place;

}

int OBC_FindFirstEmptyBit(OBC_ALLOCATOR_META_TYPE rraw){
    int j = 0;
    const OBC_ALLOCATOR_META_TYPE mask = 1 << ((sizeof(OBC_ALLOCATOR_META_TYPE)*8)-1);
    for(j = 0; j < sizeof(OBC_ALLOCATOR_META_TYPE)*8; j++){
        if( (rraw & mask) == 0 ){
            break;
        }
        rraw<<=1;
    }
    return j;
}

OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator){

    const size_t BITS = sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8;

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t i;

    if(OBC_RayDoExpand(& allocRay->backed.rawData) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t metaMaxUnits = BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits <= backedMaxUnits){

        if(OBC_RayDoExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOCATOR_MAX_CONTRACT_TRIES; i++){
                if(OBC_RayContract(& allocRay->backed.rawData) != OBC_ERROR_FAILURE){
                    return OBC_ERROR_FAILURE;
                }
            }
            return OBC_ERROR_FAILURE;
        }

    }

    //OBC_RayAddValue(&allocRay->meta.rawData,0);
    OBC_RayNewElement(& allocRay->backed.rawData);

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos){
    const size_t BITS = sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8;

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    size_t unitMask = 1<<((BITS-bits)-1);

    ((OBC_ALLOCATOR_META_TYPE*) allocRay->meta.rawData)[unit]|=unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos){
    const size_t BITS = sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8;

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    size_t unitMask = 1<<((BITS-bits)-1);
    unitMask=~unitMask;

    ((OBC_ALLOCATOR_META_TYPE*) allocRay->meta.rawData)[unit]&=unitMask;

    return OBC_ERROR_NO_OP;
}


