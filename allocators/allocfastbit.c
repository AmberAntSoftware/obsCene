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
        OBC_freeRayData(&allocator->backed);
        return NULL;
    }

    if(OBC_initRay(&allocator->available,1,sizeof(OBC_ALLOCFASTBIT_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->meta);
        OBC_freeRayData(&allocator->backed);
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
    OBC_freeRayData(& allocator_->backed);
    OBC_freeRayData(& allocator_->meta);
    OBC_freeRayData(& allocator_->available);
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

    return OBC_NULL_INDEX;

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
    if(memset(allocator_->meta.rawData+start,0,(allocator_->meta.maxLength)-start) != allocator_->meta.rawData+start){
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
    OBC_ALLOCFASTBIT_META_TYPE* data = (OBC_ALLOCFASTBIT_META_TYPE *)allocator_->meta.rawData;

    //printf("DATA:: %u    AT: %u     FROM: %u\n",data[where],where,pos);

    if(data[where] == (~((OBC_ALLOCFASTBIT_META_TYPE)0))
        //|| (pos - OBC_ALLOCFASTBIT_META_BITS) >= (allocator_->backed.maxLength*CHAR_BIT)
       ){

        //*
        size_t addTo = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->available));
        /*/
        size_t addTo = 0;
        OBC_RayNewElementFast(addTo,OBC_FROM_RAY_VAL(allocator_->available));
        //*/
        ///puts("PUSHH");

        if(addTo==OBC_NULL_INDEX){
            return OBC_ERROR_FAILURE;
        }

        data = (OBC_ALLOCFASTBIT_META_TYPE *)allocator_->available.rawData;
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
        if(allocator_->backed.maxLength == allocator_->backed.curLength){
            return OBC_NULL_INDEX;
        }
        return allocator_->backed.curLength;
    }


    OBC_ALLOCFASTBIT_META_TYPE val = 0;
    goto SKIP_INIT_POP;
    do{
        OBC_RayPopElement(OBC_FROM_RAY_VAL(allocator_->available));

        SKIP_INIT_POP:
        val =
        ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->available.rawData)
                            [allocator_->available.curUnitLength-1];
        found = val*OBC_ALLOCFASTBIT_META_BITS;

        val =
        ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->meta.rawData)[val];
    }while(val+1 == 0);

    unsigned char *raw = (unsigned char *)&val;

    size_t pos = OBC_AllocFastBitGetFirstBit(val);
    if(pos == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    found+=pos;

    if(found > allocator_->backed.curUnitLength){
        return OBC_NULL_INDEX;
    }

    //*
    raw[pos/CHAR_BIT] |= 1<<((CHAR_BIT-(pos-((pos/CHAR_BIT)*CHAR_BIT)))-1);
    if(val+1 == 0){
        OBC_RayPopElement(OBC_FROM_RAY_VAL(allocator_->available));
        //puts("POPP");
    }
    //*/

    return found;
}

size_t OBC_AllocFastBitGetFirstBit(OBC_ALLOCFASTBIT_META_TYPE val){

    unsigned char *raw = (unsigned char *)&val;

    size_t i,j;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBIT_META_TYPE); i++){
        if(raw[i]!= (unsigned char)(~0)){
            for(j=0; j < CHAR_BIT; j++){
                if((((raw[i])>>((CHAR_BIT-j)-1))&0x01) == 0){
                    return j+(i*CHAR_BIT);
                }
            }
            return OBC_NULL_INDEX;
        }
    }

    return OBC_NULL_INDEX;

}























size_t OBC_AllocFastBitMalloc2(void *allocator){

    ///due to rays being postional, filling the
    ///free list on allocation of blank space
    ///is unnecessary due to the end being at the
    ///next available spot anyway

    OBC_AllocFastBit *allocator_ = OBC_TO_RAW_ALLOCFASTBIT(allocator);
    size_t found = 0;

    if(allocator_->available.curUnitLength > 0){

        OBC_ALLOCFASTBIT_META_TYPE bitPos = 0;

        goto SKIP_POP;
        do{
            OBC_RayPopElement(allocator_->available.rawData);
            if(allocator_->available.curUnitLength == 0){
                goto EMPTY_META;
            }

SKIP_POP:
            found =
            ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->available.rawData)
                                [allocator_->available.curUnitLength-1];
            bitPos = ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->meta.rawData)
                [found];
        }while(bitPos == (~0));

        found*=OBC_ALLOCFASTBIT_META_BITS;
        bitPos = OBC_AllocFastBitGetFirstBit(bitPos);///always non ~0
        found+=bitPos;

        if(found >= allocator_->backed.maxUnitLength){
             if(OBC_AllocFastBitExpand(allocator) == OBC_NULL_INDEX){
                return OBC_NULL_INDEX;
             }
             found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));
        }

        if(found != OBC_NULL_INDEX){
            OBC_AllocFastBitMarkAllocated(allocator, found);
        }

        return found;
    }
EMPTY_META:

    if(allocator_->backed.maxUnitLength == allocator_->backed.curUnitLength){
        if(OBC_AllocFastBitExpand(allocator) == OBC_NULL_INDEX){
            return OBC_NULL_INDEX;
        }
    }

    found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));

    if(found == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    OBC_AllocFastBitMarkAllocated(allocator, found);

    return found;

}




















void **OBC_newAllocFastBit2(size_t unitSize){

    OBC_AllocFastBit2 *allocator = calloc(1,sizeof(OBC_AllocFastBit2));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocFastBit2(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}
void *OBC_initAllocFastBit2(OBC_AllocFastBit2 *allocator, size_t unitSize){

    if(OBC_initRay(&allocator->backed,0,unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(& allocator->meta,2,sizeof(OBC_ALLOCFASTBIT_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->backed);
        return NULL;
    }

    allocator->freeHead = OBC_NULL_INDEX;

    return allocator;

}

void OBC_freeAllocFastBit2(void *allocator){
    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);
    OBC_freeAllocFastBit2Data(allocator);
    free(allocator_);
}

void OBC_freeAllocFastBit2Data(void *allocator){
    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);
    OBC_freeRayData(& allocator_->backed);
    OBC_freeRayData(& allocator_->meta);
}


OBC_ERROR_ENUM OBC_AllocFastBit2Expand(void *allocator){

    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    OBC_ERROR_PROPAGATE(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->backed)));

    size_t start = allocator_->meta.maxLength;
    if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->meta)) == OBC_ERROR_FAILURE){
        if(OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->meta)) == OBC_ERROR_FAILURE){
            ///TODO serious error
        }
        return OBC_ERROR_FAILURE;
    }

    /*
    if(memset(allocator_->meta.rawData+start,0,(allocator_->meta.maxLength)-start) != allocator_->meta.rawData+start){
        ///TODO serious crisis
        return OBC_ERROR_FAILURE;
    }
    /*/
    memset(allocator_->meta.rawData+start,0,(allocator_->meta.maxLength)-start);

    return OBC_ERROR_SUCCESS;
}

/*
size_t OBC_AllocFastBit2Malloc(void *allocator){
    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);
    size_t found = OBC_NULL_INDEX;

    if(allocator_->freeHead != OBC_NULL_INDEX){

        found = OBC_AllocFastBit2GetFreeMeta(allocator);
        if(found != OBC_NULL_INDEX){
            goto MARK_DATA;
        }

        return OBC_NULL_INDEX;
    }

    if(allocator_->backed.curLength == allocator_->backed.maxLength){
        if(OBC_AllocFastBit2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));
    if(found == OBC_NULL_INDEX){
        ///serious error and impossible, so if so serious error
    }

MARK_DATA:;

    const size_t chunk = found/OBC_ALLOCFASTBIT_META_CHUNKSIZE_BITS;
    const size_t bitsIntoChunk =
        found-(chunk*OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES);
    const size_t pos =
        (chunk*(OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES*2))
        +bitsIntoChunk;
    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    const char unitMask = 1<<((CHAR_BIT-bits)-1);
    allocator_->meta.rawData[unit] |= unitMask;

    return found;
}
OBC_ERROR_ENUM OBC_AllocFastBit2Free(void *allocator, size_t pos){
    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    ///TODO remove safety
    if(pos >= (allocator_->meta.unitSize/OBC_ALLOCFASTBIT_META_CHUNKSIZE)){
        return OBC_ERROR_NO_OP;
    }

    if(allocator_->freeHead != OBC_NULL_INDEX){
        //
        return OBC_ERROR_FAILURE;
    }


MARK_DATA:;

    const size_t chunk = pos/OBC_ALLOCFASTBIT_META_CHUNKSIZE_BITS;
    const size_t bitsIntoChunk =
        pos-(chunk*OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES);
    const size_t ppos =
        (chunk*(OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES*2))
        +bitsIntoChunk;

    const size_t unit = ppos/CHAR_BIT;
    const size_t bits = (ppos-(unit*(CHAR_BIT)));
    unsigned char unitMask = ~(1<<((CHAR_BIT-bits)-1));
    allocator_->meta.rawData[unit] &= unitMask;

}

size_t OBC_AllocFastBit2GetFreeMeta(void *allocator){

    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    size_t found = allocator_->freeHead*OBC_ALLOCFASTBIT_META_CHUNKSIZE*2;//size of 1 unit
    OBC_ALLOCFASTBIT_META meta =
        ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->meta.rawData)[found+OBC_ALLOCFASTBIT_META_CHUNKSIZE];

    const size_t chunk = found/OBC_ALLOCFASTBIT_META_CHUNKSIZE_BITS;
    const size_t unitsIntoChunk =
        (pos-(chunk*OBC_ALLOCFASTBIT_META_CHUNKSIZE_BYTES))
        /OBC_ALLOCFASTBIT_META_BITS;

    found+=unitsIntoChunk;

    OBC_ALLOCFASTBIT_META meta =
        ((OBC_ALLOCFASTBIT_META_TYPE*)allocator_->meta.rawData)[found];

    if(meta+1 == 0){
        return OBC_NULL_INDEX;
    }

    unsigned char *raw = &meta;

    size_t i,j;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBIT_META_BYTES); i++){
        if(raw[i]!= (unsigned char)(~0)){
            for(j=0; j < CHAR_BIT; j++){
                if((((raw[i])>>((CHAR_BIT-j)-1))&0x01) == 0){
                    return j+(i*CHAR_BIT);
                }
            }
            return OBC_NULL_INDEX;
        }
    }

    return OBC_NULL_INDEX;
}
//*/




size_t OBC_AllocFastBit2Malloc(void *allocator){

    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    //*
    size_t found = OBC_NULL_INDEX;


    if(allocator_->freeHead != OBC_NULL_INDEX){

        found = OBC_AllocFastBit2GetFreeMeta(allocator);
        if(found != OBC_NULL_INDEX){
            allocator_->meta.rawData[found/CHAR_BIT] |=
                1<< (CHAR_BIT-(found-((found/CHAR_BIT)*CHAR_BIT))-1);

            const OBC_ALLOCFASTBIT_META_TYPE* data = (OBC_ALLOCFASTBIT_META_TYPE *)allocator_->meta.rawData;
            if(data[data[allocator_->freeHead*2]*2+1]+1 == 0){
                allocator_->freeHead--;
            }
        }

        return found;
    }

    found = OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));
    if(found == OBC_NULL_INDEX){
        ///serious error and impossible, so if so serious error
        exit(-1);
    }

    //allocator_->meta.rawData[found/CHAR_BIT] |=
    //            1<< (CHAR_BIT-(found-((found/CHAR_BIT)*CHAR_BIT))-1);

    return found;
    /*/

    //size_t found = 0;
    //OBC_RayNewElementFast(found, OBC_FROM_RAY_VAL(allocator_->backed));
    //return found;

OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->meta));
    return OBC_RayNewElement(OBC_FROM_RAY_VAL(allocator_->backed));

    //*/
}

OBC_ERROR_ENUM OBC_AllocFastBit2Free(void *allocator, size_t pos){
    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    ///TODO remove safety
    /*
    if(pos >= (allocator_->meta.unitSize/OBC_ALLOCFASTBIT_META_BITS)){
        return OBC_ERROR_NO_OP;
    }
    //*/

    const size_t chunk = pos/OBC_ALLOCFASTBIT_META_BITS;
    pos = pos-(chunk*OBC_ALLOCFASTBIT_META_BITS);
    pos = (chunk*OBC_ALLOCFASTBIT_META_BITS*2)+pos;

    allocator_->meta.rawData[pos/CHAR_BIT] |=
                1<< (CHAR_BIT-(pos-((pos/CHAR_BIT)*CHAR_BIT))-1);

    OBC_ALLOCFASTBIT_META_TYPE* data = (OBC_ALLOCFASTBIT_META_TYPE *)allocator_->meta.rawData;

    if(data[(chunk*OBC_ALLOCFASTBIT_META_BYTES*2)+1]
       != (~((OBC_ALLOCFASTBIT_META_TYPE)0))){
        return OBC_ERROR_SUCCESS;
    }

    /*
    if(allocator_->freeHead != OBC_NULL_INDEX){
        allocator_->freeHead++;
    }else{
        allocator_->freeHead = 0;
    }
    /*/
    allocator_->freeHead++;
    //*/
    data[allocator_->freeHead*2+1] = chunk;

    return OBC_ERROR_SUCCESS;

}

size_t OBC_AllocFastBit2GetFreeMeta(void *allocator){

    OBC_AllocFastBit2 *allocator_ = OBC_TO_RAW_ALLOCFASTBIT2(allocator);

    size_t found = allocator_->freeHead*2;
    OBC_ALLOCFASTBIT_META_TYPE* data = (OBC_ALLOCFASTBIT_META_TYPE *)allocator_->meta.rawData;

    ///size of 1 unit
    ///get head at position, then take stored head value meta
    OBC_ALLOCFASTBIT_META_TYPE place = data[found];
    OBC_ALLOCFASTBIT_META_TYPE meta = data[ place*2 +1 ];

    if(meta+1 == 0){
        return OBC_NULL_INDEX;
    }

    unsigned char *raw = (unsigned char *)&meta;

    size_t i,j;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBIT_META_BYTES); i++){
        if(raw[i]!= (unsigned char)(~0)){
            for(j=0; j < CHAR_BIT; j++){
                if((((raw[i])>>((CHAR_BIT-j)-1))&0x01) == 0){
                    found = j+(i*CHAR_BIT) + (place*2 +1);
                    return found;
                }
            }
            return OBC_NULL_INDEX;
        }
    }

    return OBC_NULL_INDEX;
}

