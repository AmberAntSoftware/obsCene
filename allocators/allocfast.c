#include "allocfast.h"

void **OBC_newAllocFast(size_t unitSize){

    OBC_AllocFast *allocator = calloc(1,sizeof(OBC_AllocFast));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocFast(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}
void *OBC_initAllocFast(OBC_AllocFast *allocator, size_t unitSize){

    if(OBC_initRay(&allocator->backed,0,unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(&allocator->meta,0,sizeof(OBC_ALLOCFAST_META)) == NULL){
        OBC_freeRayData(&allocator->backed);
        return NULL;
    }

    return allocator;

}

void OBC_freeAllocFast(void *allocator){
    OBC_AllocFast *allocator_ = OBC_TO_RAW_ALLOCFAST(allocator);
    OBC_freeAllocFastData(allocator);
    free(allocator_);
}
void OBC_freeAllocFastData(void *allocator){
    OBC_AllocFast *allocator_ = OBC_TO_RAW_ALLOCFAST(allocator);
    OBC_freeRayData(& allocator_->backed);
    OBC_freeRayData(& allocator_->meta);
}

size_t OBC_AllocFastMalloc(void *allocator){
    OBC_AllocFast *allocator_ = OBC_TO_RAW_ALLOCFAST(allocator);

    size_t found = 0;

    if(allocator_->meta.curUnitLength > 0){
        found = *((size_t*)OBC_RayGetLast(& allocator_->meta.rawData));
        OBC_RayPopElement(& allocator_->meta.rawData);
        return found;
    }



    if(allocator_->backed.curUnitLength> 0){
        found = allocator_->backed.curUnitLength;
    }

    OBC_RayNewElement(& allocator_->backed.rawData);

    /*OBC_RayNewElement(& allocator_->meta.rawData);
    OBC_RaySetLast(& allocator_->meta.rawData,
                   & allocator_->backed.curLength);*/

    return found;

}
OBC_ERROR_ENUM OBC_AllocFastFree(void *allocator, size_t data){
    OBC_AllocFast *allocator_ = OBC_TO_RAW_ALLOCFAST(allocator);


    /*
    if(allocator_->meta.curUnitLength > 0){
        ((OBC_ALLOCFAST_META *)allocator_->meta.rawData)[allocator_->meta.curUnitLength-1] = data;
        OBC_RayNewElement(& allocator_->meta.rawData);
    }else{
        OBC_RayNewElement(& allocator_->meta.rawData);
        ((OBC_ALLOCFAST_META *)allocator_->meta.rawData)[0] = data;
    }
    /*/

        //OBC_RayNewElement(& allocator_->meta.rawData);
        //((OBC_ALLOCFAST_META *)allocator_->meta.rawData)[allocator_->meta.curUnitLength-1] = data;

    //*/

    OBC_ALLOCFAST_META **dat = (OBC_ALLOCFAST_META **)&allocator_->meta.rawData;
    OBC_RayAddValue(dat,data);

    return OBC_ERROR_SUCCESS;
}

size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastExpand(void *allocator);

///UTILITY
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocFastMarkAllocated(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocFastMarkFreed(void *allocator, size_t pos);
