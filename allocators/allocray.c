#include "allocray.h"


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

void *OBC_initAllocRay(OBC_AllocRay *allocator, size_t unitSize){

    if(OBC_initRayComplex(&allocator->backed,0,unitSize) == OBC_ERROR_FAILURE){
        return NULL;
    }

    if(OBC_initRayComplex(&allocator->meta,0,sizeof(OBC_ALLOC_META_TYPE)) == OBC_ERROR_FAILURE){
        OBC_freeRayData(&allocator->backed);
        return NULL;
    }

    return allocator;

}

void OBC_freeAllocRay(void *allocator){
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_freeAllocRayData(allocator);
    free(allocRay);
}

void OBC_freeAllocRayData(void *allocator){
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_freeRayData(& allocRay->backed);
    OBC_freeRayData(& allocRay->meta);
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

    return place;
}

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data){

    OBC_AllocRayMarkFreed(allocator,data);

    return OBC_ERROR_NO_OP;

}

size_t OBC_AllocRayGetFreeLocation(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    size_t max = allocRay->meta.maxUnitLength;
    size_t i;
    size_t place = OBC_NULL_INDEX;

    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;
    for(i = 0; i < max; i++){
        if(raw[i]!= ~((OBC_ALLOC_META_TYPE)0)){
            break;
        }
    }

    if(i!=max){

        place = OBC_AllocRayFindFirstEmptyBit(raw[i]);
        if(place==OBC_ALLOC_META_BITS){
            return OBC_NULL_INDEX;
        }
        place+=i*OBC_ALLOC_META_BITS;
        if(place >= allocRay->backed.maxUnitLength){
            return OBC_NULL_INDEX;
        }

    }else {
        return OBC_NULL_INDEX;
    }

    return place;

}

int OBC_AllocRayFindFirstEmptyBit(OBC_ALLOC_META_TYPE rraw){

    unsigned int j;
    for(j = 0; j < OBC_ALLOC_META_BITS; j++){
        if( (rraw & OBC_ALLOC_META_MASK) == 0 ){
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

    const size_t metaMaxUnits = OBC_ALLOC_META_BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxUnitLength*allocRay->meta.unitSize;

        if(OBC_RayExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOC_MAX_CONTRACT_TRIES; i++){
                if(OBC_RayContract(& allocRay->backed.rawData) != OBC_ERROR_FAILURE){
                    return OBC_ERROR_FAILURE;
                }else{
                    break;
                }
            }
            return OBC_ERROR_FAILURE;
        }

        memset(allocRay->meta.rawData+start,0,((allocRay->meta.maxUnitLength*allocRay->meta.unitSize)-start));

    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;
    size_t unit = pos/OBC_ALLOC_META_BITS;
    size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    raw[unit] = raw[unit] | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;

    size_t unit = pos/OBC_ALLOC_META_BITS;
    size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    unitMask=~unitMask;

    raw[unit] = raw[unit] & unitMask;

    return OBC_ERROR_NO_OP;
}

