#include "hashmap.h"

static void OBC_X_HashMapIterSkipFull(OBC_HashMap *map, OBC_HashMapIterator *iter);

void **OBC_newHashMap(size_t keySize, size_t valueSize){
    return OBC_newHashMapComplex(keySize, valueSize, 0);
}
void **OBC_newHashMapComplex(size_t keySize, size_t valueSize, size_t initBuckets){
    OBC_HashMap *map = calloc(1,sizeof(OBC_HashMap));

    if(map == NULL){
        return NULL;
    }

    if(OBC_initHashMapComplex(map,keySize, valueSize, initBuckets) == NULL){
        free(map);
        return NULL;
    }

    return OBC_HashMapGetDataPointer(map);
}

void *OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize){
    return OBC_initHashMapComplex(map,keySize, valueSize, 0);
}
void *OBC_initHashMapComplex(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t initBuckets){

    if(OBC_initRay(& map->values, initBuckets, valueSize ) == NULL){
        return NULL;
    }

    if(OBC_initRay(& map->keys, initBuckets, keySize) == NULL){
        OBC_freeRayData(& map->values);
        return NULL;
    }

    if(OBC_initRay(& map->keyHashes, initBuckets, sizeof(OBC_Hash)) == NULL){
        OBC_freeRayData(& map->values);
        OBC_freeRayData(& map->keys);
        return NULL;
    }


    map->curDepth = (initBuckets != 0);

    map->itemsPerBucket = 1;

    map->buckets = 0;
    map->count = 0;

    return map;
}

void **OBC_HashMapGetDataPointer(OBC_HashMap *map){
    return (void **)OBC_FROM_RAY_VAL(map->values);
}
void **OBC_HashMapGetKeyPointerRaw(OBC_HashMap *map){
    return (void **)OBC_FROM_RAY_VAL(map->keys);
}
void **OBC_HashMapGetKeyPointer(void *arr){
    return OBC_HashMapGetKeyPointerRaw(OBC_TO_HASHMAP_PTR(arr));
}

void OBC_freeHashMap(void *arr){
    OBC_HashMap *map = OBC_TO_HASHMAP_PTR(arr);
    OBC_freeHashMapData(map);
    free(map);
}
void OBC_freeHashMapData(OBC_HashMap *map){

    OBC_freeRayData(& map->values);
    OBC_freeRayData(& map->keys);
    OBC_freeRayData(& map->keyHashes);

}



OBC_Offset OBC_HashMapNewRaw(OBC_HashMap *map, void *key, OBC_Hash hash){

    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        if(OBC_HashMapExpand(map) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    size_t pos = OBC_X_HASHMAP_HASH_TO_POSITION(hash,map);
    if(((OBC_Hash *)map->keyHashes.rawData)[pos] < OBC_X_HASHMAP_HASH_FREED){
        ///TODO FIXME
        ///OBC_Offset position = OBC_HashMapProbeBucket(map,pos);
    }

    if(pos < map->keys.maxUnitLength){

        /*
        memcpy(
               (void*)(map->keys.rawData+(pos*map->keys.unitSize))
               ,key,map->keys.unitSize);

        ((OBC_Hash *)map->keyHashes.rawData)[pos] = hash;

        return pos;
        /*/
        goto SET_DATA;
        //*/
    }

    ///TODO

SET_DATA:

    memcpy(
       (void*)(map->keys.rawData+(pos*map->keys.unitSize))
       ,key,map->keys.unitSize);

    ((OBC_Hash *)map->keyHashes.rawData)[pos] = hash;

    return pos;

}
OBC_Offset OBC_HashMapNew(void *arr, void *key, OBC_Hash hash){
    return OBC_HashMapNewRaw(OBC_TO_HASHMAP_PTR(arr),key,hash);
}

OBC_Offset OBC_HashMapNewSimpleRaw(OBC_HashMap *map, void *key){
    return OBC_HashMapNewRaw(map,key,OBC_hash(key,map->keys.unitSize));
}
OBC_Offset OBC_HashMapNewSimple(void *arr, void *key){
    return OBC_HashMapNewSimpleRaw(OBC_TO_HASHMAP_PTR(arr),key);
}


/**
///TODO FIXME
///new function uses this somewhat
///todo add in shim nonfunc call
OBC_Offset OBC_HashMapProbeBucket(OBC_HashMap *map, OBC_Offset position){
    OBC_Offset found;

    void * __restrict keys = (void *)map->keys.rawData;
    void * __restrict data = (void *)map->values.rawData;

}
//* /

//**
///TODO FIXME
OBC_Offset OBC_HashMapKeyExists(OBC_HashMap *map, void *key, OBC_Hash hash){
    OBC_Offset found;

    void * __restrict keys = (void *)map->keys.rawData;
    void * __restrict data = (void *)map->values.rawData;

}
//*/


OBC_ERROR_ENUM OBC_HashMapDoExpand(OBC_HashMap *map){
    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        return OBC_HashMapExpand(map);
    }

    return OBC_ERROR_NO_OP;
}

/***
OBC_ERROR_ENUM OBC_HashMapGetNewStuff(OBC_HashMap *map, OBC_HashMapIterator *iter){

    (iter)->iter = OBC_X_HASHMAP_HASH_TO_POSITION((iter)->hash, (map));
    (iter)->keyCmpr = 0;
    if((iter)->iter >= OBC_X_HASHMAP_HASH_EMPTY){
        (iter)->iter+=3;//prime, skip 0
    }
    for(; (iter)->iter < OBC_X_HASHMAP_HASH_EMPTY && iter->iter < (map)->keyHashes.maxUnitLength ;)
        if(((OBC_Hash *)((map)->keyHashes.rawData))[(iter)->iter] < OBC_X_HASHMAP_HASH_EMPTY)///user body here
{
            //check if keys are equal, if not keep going
            if((iter)->keyCmpr == 0){
                //key exists
                //if they don't exist, escape
                break;
            }
}
}

}
//***/

OBC_ERROR_ENUM OBC_HashMapExpand(OBC_HashMap *map){

    OBC_ERROR_ENUM status;

    if(map->buckets == 0){
        status = OBC_HashMapExpandEmpty(map);
    }else{
        status = OBC_HashMapExpandNonEmpty(map);
    }

    return status;
}

OBC_ERROR_ENUM OBC_HashMapExpandEmpty(OBC_HashMap *map){

    const size_t mapCount = OBC_X_HASHMAP_INIT_RESERVE_COUNT;

    if(OBC_initRay(& map->values, mapCount, map->values.unitSize) == NULL){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRay(& map->keys, mapCount, map->keys.unitSize) == NULL){
        OBC_freeRayData(& map->values);
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRay(& map->keyHashes, mapCount, map->keyHashes.unitSize) == NULL){
        OBC_freeRayData(& map->values);
        OBC_freeRayData(& map->keys);
        return OBC_ERROR_FAILURE;
    }

    map->buckets = OBC_X_HASHMAP_INIT_RESERVE_COUNT;
    memset(map->keyHashes.rawData,
           OBC_X_HASHMAP_HASH_EMPTY,
           sizeof(OBC_Hash)*OBC_X_HASHMAP_INIT_RESERVE_COUNT);

    map->curDepth = 1;

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_HashMapExpandNonEmpty(OBC_HashMap *map){

    const size_t hashByteCount = map->keyHashes.maxUnitLength * sizeof(OBC_Hash);

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->values)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->keys)) == OBC_ERROR_FAILURE){
        OBC_RayContract(OBC_FROM_RAY_VAL(map->values));
        return OBC_ERROR_FAILURE;
    }

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->keyHashes)) == OBC_ERROR_FAILURE){
        OBC_RayContract(OBC_FROM_RAY_VAL(map->values));
        OBC_RayContract(OBC_FROM_RAY_VAL(map->keys));
        return OBC_ERROR_FAILURE;
    }

    memset(map->keyHashes.rawData+hashByteCount,
           OBC_X_HASHMAP_HASH_EMPTY,
           hashByteCount);

    map->buckets*=2;

    ///TODO FIXME MOVE BUCKETS DOWN

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_HashMapExpandSteppes(OBC_HashMap *map){

    if(OBC_HashMapExpand(map)==OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    map->buckets/=2;
    map->curDepth*=2;

    if(OBC_HashMapAlignSteppes(map) == OBC_ERROR_FAILURE){
        ///FIXME
        ///TODO FIXME CRITICAL ERROR SOMEHOW
        exit(-11);
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}


///FIXME
///TODO first two steppes are indirections, rest are linear alignments
OBC_ERROR_ENUM OBC_HashMapAlignSteppes(OBC_HashMap *map){

    OBC_Hash * __restrict hashes = (OBC_Hash *)map->keyHashes.rawData;
    void * __restrict keys = (void *)map->keys.rawData;
    void * __restrict data = (void *)map->values.rawData;

    const size_t newBuckets = map->buckets;
    const size_t newMask = newBuckets-1;
    const size_t oldBuckets = newBuckets/2;
    const size_t oldMask = newBuckets/2-1;
    const size_t steppes = map->curDepth;

    size_t i;
    size_t newPos;
    size_t oldPosition = oldBuckets*(steppes-1);
    size_t newPosition = newBuckets*(steppes-1);

    for(; oldPosition > 0 ;){

        i = oldBuckets;
        do{
            i--;
            newPos = hashes[i+oldPosition]&newMask;
            ///TODO FIXME check upper levels
            ///change self as deleted
            ///move data to new spot
        }while(i != 0);


        oldPosition-=oldBuckets;
        newPosition-=newBuckets;
    }

    ///First level has never been touched, will not have been
    ///touched should there have been no other steppes
    ///TODO do top level move -- no checks
    /*or(; oldPosition > 0 ;){

        i = oldBuckets;
        do{
            i--;
            newPos = hashes[i+oldPosition]&newMask;
            ///TODO FIXME check upper levels
            ///change self as deleted
            ///move data to new spot
        }while(i != 0);


        oldPosition-=oldBuckets;
        newPosition-=newBuckets;
    }*/

    return OBC_ERROR_SUCCESS;
}

void OBC_HashMapNewIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        if(OBC_HashMapExpand(map)){
            iter->iter = OBC_NULL_INDEX;
            return;
        }
    }
    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash,map);
}
void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapNewIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}
void OBC_HashMapNewIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    ///pack keys together in access layers
    iter->iter+=map->buckets;
    if(iter->iter >= map->values.maxUnitLength){
        iter->iter=OBC_NULL_INDEX;
    }
}
void OBC_HashMapNewIterNext(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapNewIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}




/**********************************************/

void OBC_HashMapSetIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){


    if(iter->hash >= OBC_X_HASHMAP_HASH_FREED){
        iter->hash+=(OBC_NULL_INDEX-OBC_X_HASHMAP_HASH_FREED)+1;
    }
    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash,map);
    iter->X_endIter = iter->iter + map->itemsPerBucket;
    iter->keyCmpr = 1;//non zero

    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        if(OBC_HashMapExpand(map) == OBC_ERROR_FAILURE){
            iter->iter = OBC_NULL_INDEX;
            iter->X_endIter = OBC_NULL_INDEX;
            return;
        }
    }

    OBC_X_HashMapIterSkipFull(map,iter);

}



void OBC_HashMapSetIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}
void OBC_HashMapSetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    iter->iter++;
    OBC_X_HashMapIterSkipFull(map,iter);

    if(iter->iter == iter->X_endIter){
        ///TODO
    }

    if( iter->iter == iter->X_endIter){
        iter->X_endIter = OBC_X_HASHMAP_HASH_FREED;
        return;
    }
    if(iter->iter >= map->values.maxUnitLength){
        iter->iter=OBC_NULL_INDEX;
    }
    iter->iter++;

}
void OBC_HashMapSetIterNext(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

///TODO FIXME add error handling potentially, though may be handled in for loop syntax from macros
static void OBC_X_HashMapIterSkipFull(OBC_HashMap *map, OBC_HashMapIterator *iter){

    const OBC_Hash const *check = map->keyHashes.rawData;
    if(check[iter->iter] != iter->hash){
        do{
            if(iter->iter >= OBC_X_HASHMAP_HASH_FREED){
                iter->keyCmpr = 0;
                ///copy
                ///TODO
                iter->X_endIter = OBC_NULL_INDEX;
                return;
            }
            iter->iter++;
        }while(iter->iter != iter->X_endIter || check[iter->iter] != iter->hash);

        ///TODO
        ///expand here, and then copy if successful, else error

    }

}



























OBC_ERROR_ENUM OBC_HashMapExpandTotalBuckets(OBC_HashMap *map){

}

OBC_ERROR_ENUM OBC_HashMapExpandBucketStorage(OBC_HashMap *map){

}

OBC_ERROR_ENUM OBC_HashMapAlignBuckets(OBC_HashMap *map){

    const size_t old_max = map->buckets/2;

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->values)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->keys)) == OBC_ERROR_FAILURE){
        OBC_RayContract(OBC_FROM_RAY_VAL(map->values));
        return OBC_ERROR_FAILURE;
    }

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(map->keyHashes)) == OBC_ERROR_FAILURE){
        OBC_RayContract(OBC_FROM_RAY_VAL(map->values));
        OBC_RayContract(OBC_FROM_RAY_VAL(map->keys));
        return OBC_ERROR_FAILURE;
    }

    memset(map->keyHashes.rawData+hashByteCount,
           OBC_X_HASHMAP_HASH_EMPTY,
           hashByteCount);

    map->buckets*=2;

    ///TODO FIXME MOVE BUCKETS DOWN

    return OBC_ERROR_SUCCESS;
}



void OBC_HashMapSet__IterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr == 0){
        memcpy(map->keys.rawData + (map->keys.unitSize * iter->iter),iter->key, map->keys.unitSize);
        memcpy(map->values.rawData + (map->values.unitSize * iter->iter),iter->value, map->values.unitSize);
        iter->iter = iter->X_endIter;
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter && iter->X_storage != OBC_NULL_INDEX){
        memcpy(map->keys.rawData + (map->keys.unitSize * iter->X_storage),iter->key, map->keys.unitSize);
        memcpy(map->values.rawData + (map->values.unitSize * iter->X_storage),iter->value, map->values.unitSize);
    }
    //memcpy key
}

void OBC_HashMapSet__IterNext(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSet__IterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    iter->keyCmpr = 1;
    iter->X_storage = OBC_NULL_INDEX;

    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        ///TODO change to expand allBuckets and expand internalBuckets
        if(OBC_HashMapExpand(map) == OBC_ERROR_FAILURE){
            iter->iter = OBC_NULL_INDEX;
            ///TODO FIXME maybe allow fall through and only error on actual issue
            ///since unlss the bucket is full, most likely valid
            ///place to put data exists
            //return;
        }
    }

    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash,map);
    iter->X_endIter = iter->iter + map->itemsPerBucket;
}

void OBC_HashMapIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

