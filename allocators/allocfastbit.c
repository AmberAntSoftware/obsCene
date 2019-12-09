#include "allocfastbit.h"

void **OBC_newAllocFastBit(size_t unitSize){

    OBC_AllocFastBit *allocator = calloc(1,sizeof(OBC_AllocFastBit));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocFastBit(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}
void *OBC_initAllocFastBit(OBC_AllocFastBit *allocator, size_t unitSize){

    if(OBC_initRay(&allocator->backed,0,unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(& allocator->meta,1,sizeof(OBC_ALLOCFASTBIT_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->backed.rawData);
        return NULL;
    }

    if(OBC_initRay(&allocator->available,1,sizeof(OBC_ALLOCFASTBIT_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->meta.rawData);
        OBC_freeRayData(&allocator->backed.rawData);
        return NULL;
    }

    return allocator;

}

void OBC_freeAllocFastBit(void *allocator){
    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);
    OBC_freeAllocFastBitData(allocator);
    free(allocator_);
}

void OBC_freeAllocFastBitData(void *allocator){
    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);
    OBC_freeRayData(& allocator_->backed.rawData);
    OBC_freeRayData(& allocator_->meta.rawData);
    OBC_freeRayData(& allocator_->available.rawData);
}

/**
size_t OBC_AllocFastBitMalloc(void *allocator){

    size_t pos = OBC_AllocFastBitGetFreeLocation(allocator);
    if(pos == OBC_NULL_INDEX){
        if(OBC_AllocFastBitExpand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
        //
    }
    //
}
**/

size_t OBC_AllocFastBitGetFreeLocation(void *allocator){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    if(allocator_->available.curLength < 1){

        if(allocator_->available.curLength==allocator_->available.maxLength){
            return OBC_NULL_INDEX;
        }

        size_t pos = allocator_->backed.curUnitLength;
        OBC_RayPushElement(OBC_FROM_RAY_VAL(allocator_->backed));
        OBC_AllocFastBitMarkAllocated(allocator,pos);
        return pos;
    }

    ///TODO FIXME

}
/**
OBC_ERROR_ENUM OBC_AllocFastBitExpand(void *allocator){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    if(OBC_RayDoExpand(OBC_FROM_RAY_VAL(allocator_->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
}
**/

///UTILITY
///utility -- marks the location bit as free
void OBC_AllocFastBitMarkAllocated(void *allocator, const size_t pos){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    const char unitMask = 1<<((CHAR_BIT-bits)-1);
    //unsigned char *mmeta = (unsigned char*)(allocator_->meta.rawData);
    allocator_->meta.rawData[unit] |= unitMask;
}

///UTILITY
///utility -- marks the location bit as used
void OBC_AllocFastBitMarkFreed(void *allocator, const size_t pos){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    unitMask=~unitMask;
    unsigned char *mmeta = (unsigned char*)allocator_->meta.rawData;
    mmeta[unit] &= unitMask;
}
















OBC_ERROR_ENUM OBC_AllocFastBitExpand(void *allocator){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_AllocFastBitBalanceMeta(allocator) == OBC_ERROR_FAILURE){
        if(OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->backed)) == OBC_ERROR_FAILURE){
            ///TODO serious error
        }
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_AllocFastBitBalanceMeta(void *allocator){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    int expanded = 0;
    size_t start = allocator_->meta.maxLength;
    while(allocator_->meta.maxLength*CHAR_BIT < allocator_->backed.maxUnitLength){
        if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->meta)) == OBC_ERROR_FAILURE){
            if(OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->backed))){
                    ///TODO serious crisis
                    return OBC_ERROR_FAILURE;
                }
            do{
                if(OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->meta))){
                    ///TODO serious crisis
                    return OBC_ERROR_FAILURE;
                }
            }while(expanded--);
            return OBC_ERROR_FAILURE;
        }
        expanded++;
    }
    if(memset(allocator_->meta.rawData+start,0,(allocator_->meta.maxLength)-start) != allocator_->meta.rawData){
        ///TODO serious crisis
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}

size_t OBC_AllocFastBitMalloc(void *allocator){

    ///due to rays being postional, filling the
    ///free list on allocation of blank space
    ///is unnecessary due to the end being at the
    ///next available spot anyway

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);
    size_t found = 0;

    if(allocator_->available.curUnitLength > 0){
        //size_t found = OBC_NULL_INDEX;
        found = OBC_AllocFastBitGetAvailable(allocator);

        if(found == OBC_NULL_INDEX){
             if(OBC_AllocFastBitExpand(allocator) == OBC_NULL_INDEX){
                return OBC_NULL_INDEX;
             }
             /*
             found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));
             /*/
             OBC_RayNewElementFast(found,OBC_FROM_RAY_VAL(allocator_->backed));
             //*/
        }

        if(found != OBC_NULL_INDEX){
            OBC_AllocFastBitMarkAllocated(allocator, found);
        }

        return found;
    }

    if(allocator_->backed.maxUnitLength == allocator_->backed.curUnitLength){
        if(OBC_AllocFastBitExpand(allocator) == OBC_NULL_INDEX){
            return OBC_NULL_INDEX;
        }
    }

    /*
    found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));
    /*/
    OBC_RayNewElementFast(found,OBC_FROM_RAY_VAL(allocator_->backed));
    //*/

    /*if(OBC_AllocFastBitBalanceMeta(allocator) == OBC_ERROR_FAILURE){
        if(OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->backed)) == OBC_ERROR_FAILURE){
            ///TODO serious error
        }
        return OBC_NULL_INDEX;
    }
    //*/

    if(found == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    //*
    OBC_AllocFastBitMarkAllocated(allocator, found);
    /*/
    const size_t unit = found/CHAR_BIT;
    const size_t bits = (found-(unit*(CHAR_BIT)));
    const char unitMask = 1<<((CHAR_BIT-bits)-1);
    //unsigned char *mmeta = (unsigned char*)(allocator_->meta.rawData);
    allocator_->meta.rawData[unit] |= unitMask;
    //*/

    return found;

}

OBC_ERROR_ENUM OBC_AllocFastBitFree(void *allocator, size_t pos){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);
    size_t where = pos/OBC_ALLOCFASTBIT_META_BITS;
    OBC_ALLOCFASTBIT_META_TYPE* data = allocator_->meta.rawData;

    //printf("DATA:: %u    AT: %u     FROM: %u\n",data[where],where,pos);

    if(data[where] == (~((OBC_ALLOCFASTBIT_META_TYPE)0)) /*||
        (where*OBC_ALLOCFASTBIT_META_BITS+1)+OBC_AllocFastBitGetFirstBit(data[where]) >= allocator_->backed.curUnitLength*/){

        size_t addTo = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->available));

        if(addTo==OBC_NULL_INDEX){
            return OBC_ERROR_FAILURE;
        }

        data = allocator_->available.rawData;
        data[addTo] = where;

        ///puts("ADDED TO FREE LIST");

    }

    OBC_AllocFastBitMarkFreed(allocator,pos);

    return OBC_ERROR_SUCCESS;

}

size_t OBC_AllocFastBitGetAvailable(void *allocator){

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);

    size_t found = 0;

    if(allocator_->available.curUnitLength < 1){
debug("NOP_AVAILABL");
        return OBC_NULL_INDEX;
    }

    OBC_ALLOCFASTBIT_META_TYPE val =
    ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->available.rawData)
                        [allocator_->available.curUnitLength-1];
    found = val*OBC_ALLOCFASTBIT_META_BITS;
debug("VAL: %p\n",val);
debug("Found: %p\n",found);
    val =
    ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->meta.rawData)
                        [val];
debug("VAL: %p\n",val);

    unsigned char *raw = &val;

    size_t pos = OBC_AllocFastBitGetFirstBit(val);
    if(pos == OBC_NULL_INDEX){
debug("GETFIRST BIT ERROR");
        return OBC_NULL_INDEX;
    }
debug("FirstBit: %u\n",pos);
debug("Found: %p\n",found);
    found+=pos;
debug("Found: %p\n",found);
    if(found >= allocator_->backed.curUnitLength){
        return OBC_NULL_INDEX;
    }
    raw[pos/CHAR_BIT] |= 1<<((pos-((pos/CHAR_BIT)*CHAR_BIT)));
    if(val == (~((OBC_ALLOCFASTBIT_META_TYPE)0)) ){
        OBC_RayPopElement(OBC_FROM_RAY_VAL(allocator_->available));
    }

debug("FOUND: %u\n",found);
    if(found==OBC_NULL_INDEX){
debug("FOUND ERROR");
    }

    return found;
}

size_t OBC_AllocFastBitGetFirstBit(OBC_ALLOCFASTBIT_META_TYPE val){

    unsigned char *raw = &val;

    size_t i,j;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBIT_META_TYPE); i++){
        if(raw[i]!= (unsigned char)(~0)){
            for(j=0; j < CHAR_BIT; j++){
                if((((raw[i])>>((CHAR_BIT-j)-1))&0x01) == 0){
                    return j+(i*CHAR_BIT);
                }
            }
            printf("ERRRR %i\n",(int)raw[i]);
            return OBC_NULL_INDEX;
        }
    }

    return OBC_NULL_INDEX;

}
