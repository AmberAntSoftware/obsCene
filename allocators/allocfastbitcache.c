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

    if(OBC_initRay(&allocator->backed,0,unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRay(& allocator->meta,0,sizeof(OBC_ALLOCFASTBITCACHE_META)*2) == OBC_ERROR_FAILURE){
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

unsigned int OBC_AllocFastBitCacheMalloc(void *allocator){

    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;

    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    unsigned int newData = allocator_->metaCachePos;
    if(allocator_->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
        OBC_AllocFastBitCacheFlushToMem(allocator_);
        ///TODO grab next unit in list, apply, or new from ray
        newData = OBC_AllocFastBitCacheGetNextFree(allocator_);
        //printf("NEW_X: %u\n", newData);
    }else{

        const OBC_ALLOCFASTBITCACHE_ADDR INTERNAL = OBC_AllocFastBitCacheBitPos(allocator_->metaCache);
        /*
        const unsigned int MARK = (CHAR_BIT-1) - (newData & (CHAR_BIT-1));
        allocator_->metaCache ^= ((OBC_ALLOCFASTBITCACHE_META)1) << ((INTERNAL&(CHAR_BIT-1))+MARK);
        newData += INTERNAL;
        /*/
        ((unsigned char*)&allocator_->metaCache)[INTERNAL/CHAR_BIT] |=  (1<<(INTERNAL&(CHAR_BIT-1)));//(1<<((CHAR_BIT-1)-INTERNAL&(CHAR_BIT-1)));
        //printf("METACHACHE: %u  ::: INTERNAL: %u\n", allocator_->metaCache, INTERNAL);
        newData = INTERNAL + UNITS_PER_META*allocator_->metaCachePos;

        /***
        if(newData >= allocator_->backed.curUnitLength){
            allocator_->backed.curUnitLength++;
        }
        //***/
        //*/
    }

    if(newData >= allocator_->backed.maxUnitLength){
        if(OBC_AllocFastBitCacheExpand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    return newData;

}

OBC_ERROR_ENUM OBC_AllocFastBitCacheFree(void *allocator, unsigned int pos){

    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);
    const unsigned int UNIT = pos/UNITS_PER_META;
    const unsigned int REM = pos&(UNITS_PER_META-1);
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);

    if(UNIT == allocator_->metaCachePos){
        ((unsigned char*)&allocator_->metaCache)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));
        //puts("cache freed");
        return OBC_ERROR_SUCCESS;
    }

    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator_->meta.rawData;

    OBC_AllocFastBitCacheFlushToMem(allocator_);
    /*if(allocator_->maxHead == OBC_NULL_INDEX
       && allocator_->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
        allocator_->backed.curUnitLength += UNITS_PER_META;
        allocator_->metaCache = 0;
        allocator_->metaCachePos++;
    }*/

    OBC_ALLOCFASTBITCACHE_META localCache = meta[UNIT*2];
    OBC_ALLOCFASTBITCACHE_META toWrite = localCache;
    ((unsigned char*)&toWrite)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));

    if(localCache == OBC_ALLOCFASTBITCACHE_META_FULL){

        if(allocator_->metaCache != OBC_ALLOCFASTBITCACHE_META_FULL){
            allocator_->maxHead++;
            OBC_AllocFastBitCacheSetHeadLink(allocator_, allocator_->metaCachePos);
            /*
            OBC_ALLOCFASTBITCACHE_ADDR *head = OBC_AllocFastBitCacheGetHeadLink(allocator_);
            //allocator_->freeHead = *head;
            *head = allocator_->metaCachePos;
            //*/
        }

        //meta[allocator_->maxHead*2] = UNIT;
        //printf("ADDED UNIT: %u   AT: %u, %u\n",UNIT, allocator_->maxHead, *OBC_AllocFastBitCacheGetHeadLink(allocator_));
        allocator_->metaCache = toWrite;
        allocator_->metaCachePos = UNIT;
    }
    meta[UNIT*2] = toWrite;

    return OBC_ERROR_SUCCESS;

    /**
    OBC_ALLOCFASTBITCACHE_META localCache = meta[UNIT*2];
    OBC_ALLOCFASTBITCACHE_META toWrite = localCache;
    ((unsigned char*)&toWrite)[REM/CHAR_BIT] &= (unsigned char)~(1<<(REM&(CHAR_BIT-1)));
    meta[UNIT*2] = toWrite;

    if(localCache == OBC_ALLOCFASTBITCACHE_META_FULL){

        OBC_AllocFastBitCacheFlushToMem(allocator_);
        allocator_->maxHead++;
        //printf("headLen: %u\n",allocator_->maxHead);

        if(allocator_->maxHead == OBC_NULL_INDEX){
            //UNIT = allocator_->metaCachePos++;
        }else{
            OBC_ALLOCFASTBITCACHE_ADDR *head = OBC_AllocFastBitCacheGetHeadLink(allocator_);
            *head = UNIT;
        }

        allocator_->metaCache = toWrite;
        allocator_->metaCachePos = UNIT;

    }

    return OBC_ERROR_SUCCESS;
    //**/
}




OBC_ERROR_ENUM OBC_AllocFastBitCacheExpand(void *allocator){
    OBC_AllocFastBitCache *allocator_ = OBC_TO_RAW_ALLOCFASTBITCACHE(allocator);
    if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    if(allocator_->backed.maxUnitLength > (allocator_->meta.maxUnitLength/2)*sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT){
        if(OBC_RayExpand(OBC_FROM_RAY_VAL(allocator_->meta)) == OBC_ERROR_FAILURE){
            OBC_RayContract(OBC_FROM_RAY_VAL(allocator_->backed));
            return OBC_ERROR_FAILURE;
        }
    }
    return OBC_ERROR_SUCCESS;
}










void OBC_AllocFastBitCacheFlushToMem(OBC_AllocFastBitCache *allocator){
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    meta[allocator->metaCachePos*2] = allocator->metaCache;
    /*if(allocator->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
        allocator->maxHead--;
    }*/
}

OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetLink(OBC_AllocFastBitCache *allocator, OBC_ALLOCFASTBITCACHE_ADDR linkPos){
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);

    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    OBC_ALLOCFASTBITCACHE_ADDR *baseAddr = (OBC_ALLOCFASTBITCACHE_ADDR *)(meta+(linkPos*2 +1));
    return baseAddr + (linkPos & (NODES_PER_META-1));
}

OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetHeadLink(OBC_AllocFastBitCache *allocator){
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);

    /*
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    OBC_ALLOCFASTBITCACHE_ADDR *baseAddr = (OBC_ALLOCFASTBITCACHE_ADDR *)(meta+((allocator->maxHead)*2 +1));
    return baseAddr + (allocator->maxHead & (NODES_PER_META-1));
    /*/
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    return (OBC_ALLOCFASTBITCACHE_ADDR *)(meta + (allocator->maxHead*2)+1);
    //*/
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
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);
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
    const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    *((OBC_ALLOCFASTBITCACHE_ADDR *)(meta + ((allocator->maxHead)*2)+1)) = link;
#endif
}







unsigned int OBC_AllocFastBitCacheGetNextFree(OBC_AllocFastBitCache *allocator){

    const unsigned int UNITS_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)*CHAR_BIT;
    //const unsigned char NODES_PER_META = sizeof(OBC_ALLOCFASTBITCACHE_META)/sizeof(OBC_ALLOCFASTBITCACHE_ADDR);

    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;

    if(allocator->maxHead == OBC_NULL_INDEX){
        //allocator->maxHead = 0;
        //**
        if(allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META >= allocator->backed.curUnitLength){
            allocator->backed.curUnitLength = allocator->metaCachePos*UNITS_PER_META + UNITS_PER_META;
        }
        allocator->metaCache = 0;
        ((unsigned char *)&allocator->metaCache)[0]=1;
        allocator->metaCachePos = (allocator->backed.curUnitLength)/UNITS_PER_META;
        allocator->backed.curUnitLength++;
        return allocator->metaCachePos*UNITS_PER_META;
        /*/

        OBC_ALLOCFASTBITCACHE_ADDR maxPos = allocator->backed.curUnitLength/UNITS_PER_META;
        if(allocator->metaCachePos != maxPos){
            allocator->metaCache = meta[maxPos*2];
            allocator->metaCachePos = maxPos;
        }

        if(allocator->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
            allocator->metaCache = 0;
            allocator->backed.curUnitLength += UNITS_PER_META;
            ((unsigned char *)&allocator->metaCache)[0]=1;
            allocator->metaCachePos = (allocator->backed.curUnitLength)/UNITS_PER_META;
            allocator->backed.curUnitLength++;
        }

        return OBC_AllocFastBitCacheBitPos(allocator->metaCache) + allocator->metaCachePos*UNITS_PER_META-1;

        //*/
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

    /**

    OBC_ALLOCFASTBITCACHE_ADDR *head = OBC_AllocFastBitCacheGetHeadLink(allocator);
    OBC_ALLOCFASTBITCACHE_META *meta = (OBC_ALLOCFASTBITCACHE_META*)allocator->meta.rawData;
    //if(*head == allocator->metaCachePos){
        allocator->maxHead--;


        if(allocator->maxHead == OBC_NULL_INDEX){
            allocator->metaCachePos = allocator->backed.curUnitLength/UNITS_PER_META;
            allocator->metaCache = meta[allocator->metaCachePos*2];
            if(allocator->metaCache == OBC_ALLOCFASTBITCACHE_META_FULL){
                allocator->metaCachePos++;
                allocator->metaCache = meta[allocator->metaCachePos*2];
            }

            //printf("CurUnitPos: %u\n",allocator->backed.curUnitLength);
            //printf("MetaPos: %u\n",allocator->metaCachePos);
            //printf("MetaRaw: %x\n",meta[allocator->metaCachePos*2]);
            allocator->metaCache = meta[allocator->metaCachePos*2];
        }else{
            head = OBC_AllocFastBitCacheGetHeadLink(allocator);
            allocator->metaCachePos = *head;
        }
    //}

    allocator->metaCache = meta[allocator->metaCachePos*2];

    const unsigned int REM = OBC_AllocFastBitCacheBitPos(allocator->metaCache);
    ((unsigned char*)&allocator->metaCache)[REM/CHAR_BIT] ^=  (unsigned char)(1<<(REM&(CHAR_BIT-1)));

    return REM + allocator->metaCachePos*UNITS_PER_META;

    /*

    if(allocator->maxHead != OBC_NULL_INDEX){
        unsigned int nextNode =
            ((unsigned int*)(meta+((allocator->maxHead/NODES_PER_META)*2 +1)))[allocator->maxHead & (NODES_PER_META-1)];
        nextNode += OBC_AllocFastBitCacheBitPos(meta[nextNode*2]);
        return nextNode;
    }

    allocator->maxHead = 0;

    const OBC_ALLOCFASTBITCACHE_ADDR metaCache = (allocator->backed.curUnitLength+1)/UNITS_PER_META;
    OBC_ALLOCFASTBITCACHE_ADDR *metaEdit = ((OBC_ALLOCFASTBITCACHE_ADDR*)(meta+((allocator->maxHead/NODES_PER_META)*2 +1)));
    *metaEdit = metaCache;
    allocator->metaCachePos = metaCache;
    allocator->metaCache = meta[metaCache*2];

    return allocator->backed.curUnitLength+1;

    //*/

}

/*
unsigned int OBC_AllocFastBitCacheBitPos(OBC_ALLOCFASTBITCACHE_META meta){
    unsigned char *raw = (unsigned int*)(&meta);
    unsigned int i;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBITCACHE_META); i+=sizeof(unsigned int)){
        if(((unsigned int *)raw)[i] != (~((unsigned int)0))){
            break;
        }
    }
    raw = (unsigned char *)(((unsigned int *)raw)[i]);
    for(i = 0; i < sizeof(unsigned int); i+=sizeof(unsigned char)){
        if(raw[i] != (~((unsigned char)0))){
            break;
        }
    }
    raw = raw+i;
    i = CHAR_BIT;
    do{
        i--;
        if(((*raw)&(((CHAR_BIT-1)>>i))) != 0){
            break;
        }
    }while(i != 0);

    return (((size_t)raw) - ((size_t)&meta))+i;
}
/*/

unsigned int OBC_AllocFastBitCacheBitPos(OBC_ALLOCFASTBITCACHE_META meta){
    unsigned char *raw = (unsigned char*)(&meta);
    unsigned int i;
    for(i = 0; i < sizeof(OBC_ALLOCFASTBITCACHE_META); i+=sizeof(unsigned int)){
        if((unsigned int)(~(*((unsigned int *)(raw+i)))) != 0){
            //printf("INT CHECK: %i\n", i);
            break;
        }
    }
    raw += i;
    for(i = 0; i < sizeof(unsigned int); i+=sizeof(unsigned char)){
        if((unsigned char)(~raw[i]) != 0){
            //printf("BYTE CHECK: %i / %i\n",raw[i], (~raw[i]));
            break;
        }
    }
    raw += i;
    for(i = 0; i < CHAR_BIT; i++){
        if(((*raw)&(1<<i)) == 0){
            break;
        }
    }
    /*
    printf("BYTE_DIFF: %u\n",(((size_t)(&(*raw))) - ((size_t)(&meta))));
    unsigned int n = ((((size_t)(&(*raw))) - ((size_t)(&meta)))*CHAR_BIT)+i;
    printf("VAL: %u\n",n);
    return n;
    /*/
    return ((((size_t)(&(*raw))) - ((size_t)(&meta)))*CHAR_BIT)+i;
    //*/
}

//*/
