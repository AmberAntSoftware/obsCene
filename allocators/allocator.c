#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"

//static OBC_AllocRay hiddenAlloc;
static const size_t BITS = sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8;
static const OBC_ALLOCATOR_META_TYPE MASK = 1 << (sizeof(OBC_ALLOCATOR_META_TYPE)*sizeof(char)*8-1);

void **OBC_newAllocRay(size_t unitSize){

    OBC_AllocRay *allocator = calloc(1,sizeof(OBC_AllocRay));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocRay(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

void *OBC_initAllocRay(void *allocator, size_t unitSize){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    if(OBC_initRay(&allocRay->backed,0,unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(&allocRay->meta,0,sizeof(OBC_ALLOCATOR_META_TYPE)) == NULL){
        OBC_freeRayData(&allocRay->backed);
        return NULL;
    }

    allocRay->metaBits = 0;
    //OBC_RayAddValue(((OBC_ALLOCATOR_META_TYPE**)&allocRay->meta.rawData),0);

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

    if(place >= allocRay->backed.curUnitLength){
        OBC_RayNewElement(& (allocRay->backed.rawData));
    }

    OBC_AllocRayMarkAllocated(allocator,place);

    /*if(place > 1000){
        printf("A\n");
    }*/

    return place;
}

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    OBC_AllocRayMarkFreed(allocator,data);

    return OBC_ERROR_NO_OP;

}

size_t OBC_AllocRayGetFreeLocation(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

///FIXME based on meta length, not unit length
    size_t max = allocRay->meta.maxUnitLength;
    size_t i;
    size_t place = OBC_NULL_INDEX;

    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    for(i = 0; i < max; i++){
        if(raw[i]!= ~((OBC_ALLOCATOR_META_TYPE)0)){
            break;
        }
    }

    if(i!=max){
        place = OBC_FindFirstEmptyBit(raw[i]);
        if(place==BITS){
            return OBC_NULL_INDEX;
        }
        place+=i*BITS;
        if(place >= allocRay->backed.maxUnitLength){
            return OBC_NULL_INDEX;
        }
        //place*=allocRay->backed.unitSize;
    }else {
        return OBC_NULL_INDEX;
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

    int j;
    for(j = 0; j < BITS; j++){
        if( (rraw & MASK) == 0 ){
            break;
        }
        rraw<<=1;
    }
    return j;
}

OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t i;

    if(OBC_RayDoExpand(& allocRay->backed.rawData) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t metaMaxUnits = BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxLength;

        if(OBC_RayExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOCATOR_MAX_CONTRACT_TRIES; i++){
                if(OBC_RayContract(& allocRay->backed.rawData) != OBC_ERROR_FAILURE){
                    return OBC_ERROR_FAILURE;
                }else{
                    break;
                }
            }
            return OBC_ERROR_FAILURE;
        }

        memset(allocRay->meta.rawData+start,0,
               ((allocRay->meta.maxLength)-start));

    }

    //OBC_RayAddValue(&allocRay->meta.rawData,0);
    //OBC_RayNewElement(& allocRay->backed.rawData);

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos){

    //*
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((BITS-bits)-1);

    raw[unit] = raw[unit] | unitMask;
    //*/
    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((BITS-bits)-1);
    unitMask=~unitMask;

    allocRay->meta.rawData[unit*sizeof(OBC_ALLOCATOR_META_TYPE)] &= unitMask;

    return OBC_ERROR_NO_OP;
}


