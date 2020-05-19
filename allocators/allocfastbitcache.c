#include "allocfastbitcache.h"

void **OBC_newAllocFastBitCache(size_t unitSize){

    OBC_AllocFastBitCache *allocator = calloc(1,sizeof(OBC_AllocFastBitCache));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocFastBitCache(allocator, unitSize) == OBC_ERROR_FAILURE){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}
OBC_ERROR_ENUM OBC_initAllocFastBitCache(OBC_AllocFastBitCache *allocator, size_t unitSize){
    return OBC_initAllocFastBitCacheComplex(allocator,0,unitSize);
}

OBC_ERROR_ENUM OBC_initAllocFastBitCacheComplex(OBC_AllocFastBitCache *allocator, unsigned int initialReserveCount , unsigned int unitSize){

    if(OBC_initRayComplex(&allocator->backed,initialReserveCount,unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& allocator->meta,initialReserveCount,sizeof(OBC_ALLOCFASTBITCACHE_META)*2) == OBC_ERROR_FAILURE){
        OBC_freeRayData(&allocator->backed);
        return OBC_ERROR_FAILURE;
    }

    allocator->maxHead = OBC_NULL_INDEX;

    allocator->metaCache = 0;
    allocator->metaCachePos = 0;

    return OBC_ERROR_SUCCESS;

}




void OBC_freeAllocFastBitCache(void *allocator){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    OBC_freeAllocFastBitCacheData(allocator);
    free(allocator_);
}

void OBC_freeAllocFastBitCacheData(void *allocator){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    OBC_freeRayData(& allocator_->meta);
    OBC_freeRayData(& allocator_->backed);
}

void **OBC_AllocFastBitCacheGetDataPointer(OBC_AllocFastBitCache *allocator){
    return (void **)OBC_FROM_RAY_VAL(allocator->backed);
}


unsigned int OBC_AllocFastBitCacheMalloc(void *allocator){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    return OBC_AllocFastBitCacheMallocRaw(allocator_);
}


unsigned int OBC_AllocFastBitCacheMallocRaw(OBC_AllocFastBitCache *allocator){
    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;

    unsigned int newData = allocator->metaCachePos;
    if(allocator->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
        OBC_AllocFastBitCacheFlushToMem(allocator);
        newData = OBC_AllocFastBitCacheGetNextFree(allocator);
    }else{

        const OBC_ALLOCFASTBITCACHE_ADDR INTERNAL = OBC_AllocFastBitCacheBitPos(allocator->metaCache);
        ((unsigned char*)&allocator->metaCache)[INTERNAL/CHAR_BIT] |=  (1<<(INTERNAL&(CHAR_BIT-1)));
        newData = INTERNAL + UNITS_PER_META*allocator->metaCachePos;
    }

    if(newData >= allocator->backed.maxUnitLength){
        if(OBC_AllocFastBitCacheExpandRaw(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    return newData;
}

OBC_ERROR_ENUM OBC_AllocFastBitCacheFree(void *allocator, unsigned int pos){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    return OBC_AllocFastBitCacheFreeRaw(allocator_,pos);
}

OBC_ERROR_ENUM OBC_AllocFastBitCacheFreeRaw(OBC_AllocFastBitCache *allocator, size_t pos){
    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;
    const unsigned int UNIT = pos/UNITS_PER_META;
    const unsigned int REM = pos&(UNITS_PER_META-1);

    if(UNIT == allocator->metaCachePos){
        ((unsigned char*)&allocator->metaCache)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));
        return OBC_ERROR_SUCCESS;
    }

    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;

    OBC_AllocFastBitCacheFlushToMem(allocator);

    OBC_ALLOCFASTBITCACHE_META localCache = meta[UNIT*2];
    OBC_ALLOCFASTBITCACHE_META toWrite = localCache;
    ((unsigned char*)&toWrite)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));

    if(localCache == OBC_ALLOCFASTBITCACHE_META_FULL){

        if(allocator->metaCache != OBC_ALLOCFASTBITCACHE_META_FULL){
            allocator->maxHead++;
            OBC_AllocFastBitCacheSetHeadLink(allocator, allocator->metaCachePos);
        }

        allocator->metaCache = toWrite;
        allocator->metaCachePos = UNIT;
    }
    meta[UNIT*2] = toWrite;

    return OBC_ERROR_SUCCESS;
}





OBC_ERROR_ENUM OBC_AllocFastBitCacheExpand(void *allocator){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    return OBC_AllocFastBitCacheExpandRaw(allocator_);
}

OBC_ERROR_ENUM OBC_AllocFastBitCacheExpandRaw(OBC_AllocFastBitCache *allocator){
    if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    if(allocator->backed.maxUnitLength > (allocator->meta.maxUnitLength/2)*sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT){
        if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator->meta)) == OBC_ERROR_FAILURE){
            OBC_RayContract(OBC_FROM_RAY_VAL(allocator->backed));
            return OBC_ERROR_FAILURE;
        }
    }
    return OBC_ERROR_SUCCESS;
}







void OBC_AllocFastBitCacheFlushToMem(OBC_AllocFastBitCache *allocator){
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    meta[allocator->metaCachePos*2] = allocator->metaCache;
}

OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetLink(OBC_AllocFastBitCache *allocator, OBC_ALLOCFASTBITCACHE_ADDR linkPos){
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);

    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    OBC_ALLOCFASTBITCACHE_ADDR *baseAddr = (OBC_ALLOCFASTBITCACHE_ADDR *)(meta+(linkPos*2 +1));
    return baseAddr + (linkPos & (NODES_PER_META-1));
}

OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetHeadLink(OBC_AllocFastBitCache *allocator){
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    return (OBC_ALLOCFASTBITCACHE_ADDR *)(meta + (allocator->maxHead*2)+1);
}

OBC_ALLOCFASTBITCACHE_ADDR OBC_AllocFastBitCacheXetHeadLink(OBC_AllocFastBitCache *allocator){
#ifdef OBC_ALLOCFASTBITCACHE_STACK_SIZE
    if(allocator->maxHead < OBC_ALLOCFASTBITCACHE_STACK_SIZE){
        return allocator->freeStack[allocator->maxHead];
    }else{
        const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);
        OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
        return *((OBC_ALLOCFASTBITCACHE_ADDR *)(meta + ((allocator->maxHead-OBC_ALLOCFASTBITCACHE_STACK_SIZE)*2)+1));
    }
#else
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    return *((OBC_ALLOCFASTBITCACHE_ADDR *)(meta + ((allocator->maxHead)*2)+1));
#endif
}

void OBC_AllocFastBitCacheSetHeadLink(OBC_AllocFastBitCache *allocator, OBC_ALLOCFASTBITCACHE_ADDR link){
#ifdef OBC_ALLOCFASTBITCACHE_STACK_SIZE
    if(allocator->maxHead < OBC_ALLOCFASTBITCACHE_STACK_SIZE){
        allocator->freeStack[allocator->maxHead] = link;
    }else{
        const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);
        OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
        *((OBC_ALLOCFASTBITCACHE_ADDR *)(meta + ((allocator->maxHead-OBC_ALLOCFASTBITCACHE_STACK_SIZE)*2)+1)) = link;
    }
#else
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    *((OBC_ALLOCFASTBITCACHE_ADDR *)(meta + ((allocator->maxHead)*2)+1)) = link;
#endif
}







unsigned int OBC_AllocFastBitCacheGetNextFree(OBC_AllocFastBitCache *allocator){

    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;

    if(allocator->maxHead == OBC_NULL_INDEX){
        if(allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META >= allocator->backed.curUnitLength){
            allocator->backed.curUnitLength = allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META;
        }
        allocator->metaCache = 0;
        ((unsigned char *)&allocator->metaCache)[0]=1;
        allocator->metaCachePos = (allocator->backed.curUnitLength)/UNITS_PER_META;
        allocator->backed.curUnitLength++;
        return allocator->metaCachePos*UNITS_PER_META;
    }

    OBC_ALLOCFASTBITCACHE_ADDR link = OBC_AllocFastBitCacheXetHeadLink(allocator);
    allocator->maxHead--;

    allocator->metaCachePos = link;
    allocator->metaCache = meta[link*2];

    OBC_ALLOCFASTBITCACHE_ADDR pos = OBC_AllocFastBitCacheBitPos(allocator->metaCache);
    const unsigned int REM = pos&(UNITS_PER_META-1);
    ((unsigned char*)&allocator->metaCache)[REM/CHAR_BIT] ^= (unsigned char)(1<<(REM&(CHAR_BIT-1)));

    pos += UNITS_PER_META*link;

    return pos;

}

unsigned int OBC_AllocFastBitCacheBitPos(OBC_ALLOCFASTBITCACHE_META meta){
    unsigned char *raw = (unsigned char*)(&meta);
    unsigned int i;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBITCACHE_META); i+=sizeof(unsigned int)){
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
