#include "hashmap.h"

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









OBC_ERROR_ENUM OBC_X_HashMapExpandEmpty(OBC_HashMap *map){

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

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_X_HashMapExpandNonEmpty(OBC_HashMap *map){

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

    return OBC_ERROR_SUCCESS;
}































OBC_ERROR_ENUM OBC_HashMapExpandBucketSize(OBC_HashMap *map){

    map->itemsPerBucket*=2;
    if(OBC_X_HashMapExpand(map)==OBC_ERROR_FAILURE){
        map->itemsPerBucket/=2;
        return OBC_ERROR_FAILURE;
    }
    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_HashMapExpandBucketCount(OBC_HashMap *map){


    map->buckets*=2;
    if(OBC_X_HashMapExpand(map)==OBC_ERROR_FAILURE){
        map->buckets/=2;
        return OBC_ERROR_FAILURE;
    }
    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_X_HashMapExpand(OBC_HashMap *map){

    const size_t old_size = map->values.maxUnitLength;

    if(OBC_X_HashMapHowExpand(map) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    OBC_Hash *const hashes = (OBC_Hash *const)map->keyHashes.rawData;
    void *const values = (void *const)map->values.rawData;
    void *const keys = (void *const)map->keys.rawData;
    const size_t keySize = map->keys.unitSize;
    const size_t valueSize = map->values.unitSize;
    const size_t offset = old_size-1;
    OBC_Offset newOff;

    size_t i;
    for(i = 0; i < old_size; i++){

        if(hashes[offset-i] < OBC_X_HASHMAP_HASH_FREED){
            newOff = OBC_X_HASHMAP_HASH_TO_POSITION(hashes[offset-i],map) * map->itemsPerBucket;

            if(hashes[newOff] < OBC_X_HASHMAP_HASH_FREED){
                do{
                    newOff++;
                }while(hashes[newOff] < OBC_X_HASHMAP_HASH_FREED);
            }
            hashes[newOff] = hashes[offset-i];
            memcpy(keys+(keySize*newOff),keys+((offset-i)*keySize),keySize);
            memcpy(values+(keySize*newOff),values+((offset-i)*valueSize),valueSize);
        }

    }

    return OBC_ERROR_SUCCESS;

}

OBC_ERROR_ENUM OBC_X_HashMapHowExpand(OBC_HashMap *map){
    OBC_ERROR_ENUM status;

    if(map->buckets == 0){
        status = OBC_X_HashMapExpandEmpty(map);
    }else{
        status = OBC_X_HashMapExpandNonEmpty(map);
    }

    return status;
}














void OBC_HashMapSetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr == 0){
        memcpy(map->keys.rawData + (map->keys.unitSize * iter->iter),iter->key, map->keys.unitSize);
        memcpy(map->values.rawData + (map->values.unitSize * iter->iter),iter->value, map->values.unitSize);
        iter->X_storage = iter->iter;
        iter->iter = iter->X_endIter;
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter){
        if(iter->X_storage == OBC_NULL_INDEX){
            if(OBC_HashMapExpandBucketSize(map) == OBC_ERROR_FAILURE){
                iter->X_storage = OBC_NULL_INDEX;
            }else{
                OBC_HashMapSetIterStartRaw(map,iter);
            }
        }else{
            memcpy(map->keys.rawData + (map->keys.unitSize * iter->X_storage),iter->key, map->keys.unitSize);
            memcpy(map->values.rawData + (map->values.unitSize * iter->X_storage),iter->value, map->values.unitSize);
        }
    }
    //memcpy key
}

void OBC_HashMapSetIterNext(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapSetIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    iter->keyCmpr = 1;
    iter->X_storage = OBC_NULL_INDEX;

    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        ///TODO change to expand allBuckets and expand internalBuckets
        if(OBC_HashMapExpandBucketCount(map) == OBC_ERROR_FAILURE){
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

void OBC_HashMapSetIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}









void OBC_HashMapPutIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr == 0){
        iter->iter = iter->X_endIter;
        iter->X_storage = OBC_NULL_INDEX;
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter){
        if(iter->X_storage == OBC_NULL_INDEX){
            if(OBC_HashMapExpandBucketSize(map) == OBC_ERROR_FAILURE){
                iter->X_storage = OBC_NULL_INDEX;
            }else{
                OBC_HashMapPutIterStart(map,iter);
            }
        }else{
            memcpy(map->keys.rawData + (map->keys.unitSize * iter->X_storage),iter->key, map->keys.unitSize);
            memcpy(map->values.rawData + (map->values.unitSize * iter->X_storage),iter->value, map->values.unitSize);
        }
    }
    //memcpy key
}

void OBC_HashMapPutIterNext(void *arr, OBC_HashMapIterator *iter){
     OBC_HashMapPutIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapPutIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}









void OBC_HashMapGetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr == 0){
        iter->X_endIter = iter->iter;
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter){
        iter->iter = OBC_NULL_INDEX;
        iter->X_endIter = OBC_NULL_INDEX;
    }

}


void OBC_HashMapGetIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    iter->keyCmpr = 1;
    iter->X_storage = OBC_NULL_INDEX;
    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash,map);
    iter->X_endIter = iter->iter + map->itemsPerBucket;
}

void OBC_HashMapGetIterNext(void *arr, OBC_HashMapIterator *iter){
     OBC_HashMapGetIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapGetIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapGetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}












void OBC_HashMapNewIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr == 0){
        iter->iter = OBC_NULL_INDEX;
        iter->X_endIter = iter->iter;
        return;
    }

    if(iter->X_storage != OBC_NULL_INDEX){
        iter->X_endIter = iter->X_storage;
        iter->iter = iter->X_endIter;
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter){

        if(iter->X_storage == OBC_NULL_INDEX){
            if(OBC_HashMapExpandBucketSize(map) == OBC_ERROR_FAILURE){
                iter->X_storage = OBC_NULL_INDEX;
                iter->iter = OBC_NULL_INDEX;
                iter->X_endIter = OBC_NULL_INDEX;
            }else{
                OBC_HashMapSetIterStartRaw(map,iter);
            }
        }

    }

}

void OBC_HashMapNewIterNext(void *arr, OBC_HashMapIterator *iter){
     OBC_HashMapNewIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}
