#include "allocray.h"

void **OBC_newAllocRay(size_t unitSize){

    OBC_AllocRay *allocRay = calloc(1,sizeof(OBC_AllocRay));

    if(allocRay == NULL){
        return NULL;
    }

    if(OBC_initAllocRay(allocRay, unitSize) == OBC_ERROR_FAILURE){
        free(allocRay);
        return NULL;
    }

    return OBC_AllocRayGetDataPointer(allocRay);

}

OBC_ERROR_ENUM OBC_initAllocRay(OBC_AllocRay *allocRay, size_t unitSize){

    if(OBC_initRayComplex(&allocRay->backed, 0, unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(&allocRay->meta, 0, sizeof(OBC_ALLOC_META_TYPE)) == OBC_ERROR_FAILURE){
        OBC_freeRayData(&allocRay->backed);
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}

void **OBC_AllocRayGetDataPointer(OBC_AllocRay *allocator){
    return (void**)OBC_FROM_RAY_VAL(allocator->backed);
}

void OBC_freeAllocRay(void *allocator){
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_freeAllocRayData(allocRay);
    free(allocRay);
}

void OBC_freeAllocRayData(OBC_AllocRay *allocRay){
    OBC_freeRayData(& allocRay->backed);
    OBC_freeRayData(& allocRay->meta);
}


OBC_Offset OBC_AllocRayMalloc(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    return OBC_AllocRayMallocRaw(allocRay);
}

OBC_Offset OBC_AllocRayMallocRaw(OBC_AllocRay *allocRay){

    OBC_Offset place = OBC_AllocRayGetFreeLocationRaw(allocRay);
    if(place == OBC_NULL_INDEX){
        if(OBC_AllocRayExpandRaw(allocRay) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        place = allocRay->backed.curUnitLength;
    }

    if(place >= allocRay->backed.curUnitLength){
        OBC_RayNewElementRaw(& allocRay->backed);
    }

    OBC_AllocRayMarkAllocatedRaw(allocRay,place);

    return place;
}

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, OBC_Offset index){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    return OBC_AllocRayFreeRaw(allocRay, index);
}

OBC_ERROR_ENUM OBC_AllocRayFreeRaw(OBC_AllocRay *allocRay, OBC_Offset index){

    OBC_AllocRayMarkFreedRaw(allocRay, index);
    return OBC_ERROR_NO_OP;
}

OBC_Offset OBC_AllocRayGetFreeLocation(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    return OBC_AllocRayGetFreeLocationRaw(allocRay);
}

OBC_Offset OBC_AllocRayGetFreeLocationRaw(OBC_AllocRay *allocRay){

    OBC_Offset max = allocRay->meta.maxUnitLength;
    OBC_Offset i;
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
    return OBC_AllocRayExpandRaw(allocRay);
}

OBC_ERROR_ENUM OBC_AllocRayExpandRaw(OBC_AllocRay *allocRay){

    size_t i;

    if(OBC_RayDoExpand(& allocRay->backed.rawData) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t metaMaxUnits = OBC_ALLOC_META_BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxUnitLength*allocRay->meta.unitSize;

        if(OBC_RayExpandRaw(& allocRay->meta) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOC_MAX_CONTRACT_TRIES; i++){
                if(OBC_RayContractRaw(& allocRay->backed) != OBC_ERROR_FAILURE){
                    return OBC_ERROR_FAILURE;
                }else{
                    break;
                }
            }
            return OBC_ERROR_FAILURE;
        }

        memset(allocRay->meta.rawData+start, 0,
               ((allocRay->meta.maxUnitLength*allocRay->meta.unitSize)-start));

    }

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, OBC_Offset index){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    return OBC_AllocRayMarkAllocatedRaw(allocRay, index);
}

OBC_ERROR_ENUM OBC_AllocRayMarkAllocatedRaw(OBC_AllocRay *allocRay, OBC_Offset index){

    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;
    size_t unit = index/OBC_ALLOC_META_BITS;
    size_t bits = (index-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    raw[unit] = raw[unit] | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, OBC_Offset index){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    return OBC_AllocRayMarkFreedRaw(allocRay, index);
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreedRaw(OBC_AllocRay *allocRay, OBC_Offset index){

    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;

    size_t unit = index/OBC_ALLOC_META_BITS;
    size_t bits = (index-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    unitMask=~unitMask;

    raw[unit] = raw[unit] & unitMask;

    return OBC_ERROR_NO_OP;
}

