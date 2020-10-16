#include "alloclistbit.h"

void **OBC_newAllocListBit(size_t unitSize){

    OBC_AllocListBit *allocator = calloc(1, sizeof(OBC_AllocListBit));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocListBit(allocator, unitSize) == OBC_ERROR_FAILURE){
        free(allocator);
        return NULL;
    }

    return OBC_AllocListBitGetDataPointer(allocator);

}
OBC_ERROR_ENUM OBC_initAllocListBit(OBC_AllocListBit *allocator, size_t unitSize){
    return OBC_initAllocListBitComplex(allocator, 0, unitSize);
}

OBC_ERROR_ENUM OBC_initAllocListBitComplex(OBC_AllocListBit *allocator, OBC_Offset initialReserveCount, size_t unitSize){

    if(OBC_initRayDynamic(&allocator->backed,initialReserveCount,unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayDynamic(& allocator->meta,initialReserveCount, sizeof(OBC_ALLOCLISTBIT_META)*2) == OBC_ERROR_FAILURE){
        OBC_freeRayData(&allocator->backed);
        return OBC_ERROR_FAILURE;
    }

    allocator->maxHead = OBC_NULL_INDEX;

    allocator->metaCache = 0;
    allocator->metaCachePos = 0;

    return OBC_ERROR_SUCCESS;

}




void OBC_freeAllocListBit(void *allocator){
    OBC_AllocListBit *allocator_ = OBC_TO_RAW_ALLOCLISTBIT(allocator);
    OBC_freeAllocListBitData(allocator_);
    free(allocator_);
}

void OBC_freeAllocListBitData(OBC_AllocListBit *allocator){
    OBC_freeRayData(& allocator->meta);
    OBC_freeRayData(& allocator->backed);
}

void **OBC_AllocListBitGetDataPointer(OBC_AllocListBit *allocator){
    return (void **)OBC_FROM_RAY_VAL(allocator->backed);
}


OBC_Offset OBC_AllocListBitMalloc(void *allocator){
    OBC_AllocListBit *allocator_ = OBC_TO_RAW_ALLOCLISTBIT(allocator);
    return OBC_AllocListBitMallocRaw(allocator_);
}


OBC_Offset OBC_AllocListBitMallocRaw(OBC_AllocListBit *allocator){
    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCLISTBIT_META)*CHAR_BIT;

    unsigned int newData = allocator->metaCachePos;
    if(allocator->metaCache == OBC_ALLOCLISTBIT_META_FULL){
        OBC_AllocListBitFlushToMem(allocator);
        newData = OBC_AllocListBitGetNextFree(allocator);
    }else{

        const OBC_ALLOCLISTBIT_ADDR INTERNAL = OBC_AllocListBitBitPos(allocator->metaCache);
        ((unsigned char*)&allocator->metaCache)[INTERNAL/CHAR_BIT] |=  (1<<(INTERNAL&(CHAR_BIT-1)));
        newData = INTERNAL + UNITS_PER_META*allocator->metaCachePos;
    }

    if(newData >= allocator->backed.maxUnitLength){
        if(OBC_AllocListBitExpandRaw(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    return newData;
}

OBC_ERROR_ENUM OBC_AllocListBitFree(void *allocator, OBC_Offset index){
    OBC_AllocListBit *allocator_ = OBC_TO_RAW_ALLOCLISTBIT(allocator);
    return OBC_AllocListBitFreeRaw(allocator_, index);
}

OBC_ERROR_ENUM OBC_AllocListBitFreeRaw(OBC_AllocListBit *allocator, OBC_Offset index){
    const OBC_Offset UNITS_PER_META = sizeof(OBC_ALLOCLISTBIT_META)*CHAR_BIT;
    const OBC_Offset UNIT = index/UNITS_PER_META;
    const OBC_Offset REM = index&(UNITS_PER_META-1);

    if(UNIT == allocator->metaCachePos){
        ((unsigned char*)&allocator->metaCache)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));
        return OBC_ERROR_SUCCESS;
    }

    OBC_ALLOCLISTBIT_META *meta = (OBC_ALLOCLISTBIT_META*)allocator->meta.rawData;

    OBC_AllocListBitFlushToMem(allocator);

    OBC_ALLOCLISTBIT_META localCache = meta[UNIT*2];
    OBC_ALLOCLISTBIT_META toWrite = localCache;
    ((unsigned char*)&toWrite)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));

    if(localCache == OBC_ALLOCLISTBIT_META_FULL){

        if(allocator->metaCache != OBC_ALLOCLISTBIT_META_FULL){
            allocator->maxHead++;
            OBC_AllocListBitSetHeadLink(allocator, allocator->metaCachePos);
        }

        allocator->metaCache = toWrite;
        allocator->metaCachePos = UNIT;
    }
    meta[UNIT*2] = toWrite;

    return OBC_ERROR_SUCCESS;
}





OBC_ERROR_ENUM OBC_AllocListBitCacheExpand(void *allocator){
    OBC_AllocListBit *allocator_ = OBC_TO_RAW_ALLOCLISTBIT(allocator);
    return OBC_AllocListBitExpandRaw(allocator_);
}

OBC_ERROR_ENUM OBC_AllocListBitExpandRaw(OBC_AllocListBit *allocator){
    if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    if(allocator->backed.maxUnitLength > (allocator->meta.maxUnitLength/2)*sizeof(OBC_ALLOCLISTBIT_META)*CHAR_BIT){
        if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator->meta)) == OBC_ERROR_FAILURE){
            OBC_RayContract(OBC_FROM_RAY_VAL(allocator->backed));
            return OBC_ERROR_FAILURE;
        }
    }
    return OBC_ERROR_SUCCESS;
}







void OBC_AllocListBitFlushToMem(OBC_AllocListBit *allocator){

    OBC_ALLOCLISTBIT_META *meta = (OBC_ALLOCLISTBIT_META*)allocator->meta.rawData;
    meta[allocator->metaCachePos*2] = allocator->metaCache;
}

OBC_ALLOCLISTBIT_ADDR OBC_AllocListBitGetHeadLink(OBC_AllocListBit *allocator){

    OBC_ALLOCLISTBIT_META *meta = (OBC_ALLOCLISTBIT_META*)allocator->meta.rawData;
    return *((OBC_ALLOCLISTBIT_ADDR *)(meta + ((allocator->maxHead)*2)+1));
}

void OBC_AllocListBitSetHeadLink(OBC_AllocListBit *allocator, OBC_ALLOCLISTBIT_ADDR link){

    OBC_ALLOCLISTBIT_META *meta = (OBC_ALLOCLISTBIT_META*)allocator->meta.rawData;
    *((OBC_ALLOCLISTBIT_ADDR *)(meta + ((allocator->maxHead)*2)+1)) = link;
}







unsigned int OBC_AllocListBitGetNextFree(OBC_AllocListBit *allocator){

    const OBC_Offset UNITS_PER_META = sizeof(OBC_ALLOCLISTBIT_META)*CHAR_BIT;
    OBC_ALLOCLISTBIT_META *meta = (OBC_ALLOCLISTBIT_META*)allocator->meta.rawData;

    if(allocator->maxHead == OBC_NULL_INDEX){
        //*
        if(allocator->backed.curUnitLength + UNITS_PER_META >= allocator->backed.curUnitLength){
            allocator->backed.curUnitLength += UNITS_PER_META;
            //goto VARG;
        }
        /*/
        OBC_ALLOCLISTBIT_META dcache = meta[(a)*2];
        if(dcache != OBC_ALLOCLISTBIT_META_FULL){
            allocator->metaCachePos = allocator->backed.curUnitLength/UNITS_PER_META;
            allocator->metaCache = dcache;
            goto META_CALC;
            //return;
        }
        if(allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META >= allocator->backed.curUnitLength){
            allocator->backed.curUnitLength = allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META;
        }//*/
//VARG:
        allocator->metaCache = 0;
        ((unsigned char *)&allocator->metaCache)[0]=1;
        allocator->metaCachePos = (allocator->backed.curUnitLength)/UNITS_PER_META;
        ///allocator->backed.curUnitLength++;
        return allocator->metaCachePos*UNITS_PER_META;
    }

    OBC_ALLOCLISTBIT_ADDR link = OBC_AllocListBitGetHeadLink(allocator);
    allocator->maxHead--;

    allocator->metaCachePos = link;
    allocator->metaCache = meta[link*2];

///META_CALC:;

    OBC_ALLOCLISTBIT_ADDR pos = OBC_AllocListBitBitPos(allocator->metaCache);
    const OBC_Offset REM = pos&(UNITS_PER_META-1);
    ((unsigned char*)&allocator->metaCache)[REM/CHAR_BIT] ^= (unsigned char)(1<<(REM&(CHAR_BIT-1)));

    pos += UNITS_PER_META*link;

    return pos;

}

unsigned int OBC_AllocListBitBitPos(OBC_ALLOCLISTBIT_META meta){

    //*
    unsigned char *raw = (unsigned char*)(&meta);
    unsigned int i;
    for(i = 0; i < sizeof(OBC_ALLOCLISTBIT_META); i+=sizeof(unsigned int)){
        if((unsigned int)(~(*((unsigned int *)(raw+i)))) != 0){
            break;
        }
    }
    raw += i;
    for(i = 0; i < sizeof(unsigned int); i+=sizeof(unsigned char)){
        if((unsigned char)(~raw[i]) != 0){
            break;
        }
    }
    raw += i;
    for(i = 0; i < CHAR_BIT; i++){
        if(((*raw)&(1<<i)) == 0){
            break;
        }
    }
    return ((((size_t)(&(*raw))) - ((size_t)(&meta)))*CHAR_BIT)+i;
}
