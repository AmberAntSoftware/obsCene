#include "alloclist.h"

void **OBC_newAllocList(size_t unitSize){

    OBC_AllocList *allocator = calloc(1, sizeof(OBC_AllocList));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocList(allocator, unitSize) == OBC_ERROR_FAILURE){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

OBC_ERROR_ENUM OBC_initAllocList(OBC_AllocList *allocator, size_t unitSize){

    if(OBC_initRayComplex(& allocator->backed, 0, unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& allocator->meta, 0, sizeof(OBC_ALLOCLIST_META)) == OBC_ERROR_FAILURE){
        OBC_freeRayData(& allocator->backed);
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;

}

void OBC_freeAllocList(void *allocator){
    OBC_AllocList *allocator_ = OBC_TO_RAW_ALLOCLIST(allocator);
    OBC_freeAllocListData(allocator_);
    free(allocator_);
}

void OBC_freeAllocListData(OBC_AllocList *allocator){
    OBC_freeRayData(& allocator->backed);
    OBC_freeRayData(& allocator->meta);
}

void **OBC_AllocListGetDataPointer(OBC_AllocList *allocator){
    return (void **)OBC_FROM_RAY_VAL(allocator->backed);
}


OBC_Offset OBC_AllocListMalloc(void *allocator){

    OBC_AllocList *allocator_ = OBC_TO_RAW_ALLOCLIST(allocator);
    return OBC_AllocListMallocRaw(allocator_);
}

OBC_Offset OBC_AllocListMallocRaw(OBC_AllocList *allocator){

    OBC_Offset found = 0;

    if(allocator->meta.curUnitLength > 0){
        found = *((OBC_ALLOCLIST_META*)OBC_RayGetLastRaw(& allocator->meta));
        OBC_RayPopElementRaw(& allocator->meta);
        return found;
    }

    if(allocator->backed.curUnitLength> 0){
        found = allocator->backed.curUnitLength;
    }

    OBC_RayNewElementRaw(& allocator->backed);

    /*OBC_RayNewElement(& allocator_->meta.rawData);
    OBC_RaySetLast(& allocator_->meta.rawData,
                   & allocator_->backed.curLength);*/

    return found;

}

OBC_ERROR_ENUM OBC_AllocListFree(void *allocator, OBC_Offset data){

    OBC_AllocList *allocator_ = OBC_TO_RAW_ALLOCLIST(allocator);
    return OBC_AllocListFreeRaw(allocator_, data);
}

OBC_ERROR_ENUM OBC_AllocListFreeRaw(OBC_AllocList *allocator, OBC_Offset data){


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

    OBC_ALLOCLIST_META **dat = (OBC_ALLOCLIST_META **)&allocator->meta.rawData;
    OBC_RayAddValue(dat,data);

    return OBC_ERROR_SUCCESS;
}

/*
size_t OBC_AllocRayGetFreeLocation(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastExpand(void *allocator);

///UTILITY
///utility -- marks the location bit as used
OBC_ERROR_ENUM OBC_AllocFastMarkAllocated(void *allocator, size_t pos);
///UTILITY
///utility -- marks the location bit as free
OBC_ERROR_ENUM OBC_AllocFastMarkFreed(void *allocator, size_t pos);
*/
