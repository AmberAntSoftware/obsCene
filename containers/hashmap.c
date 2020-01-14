#include "hashmap.h"

void **OBC_newHashMap(size_t keySize, size_t valueSize){
    return OBC_newHashMapDepth(keySize, valueSize, OBC_X_HASHMAP_DEFAULT_DEPTH);
}
void **OBC_newHashMapDepth(size_t keySize, size_t valueSize, size_t bucketDepth){
    OBC_HashMap *map = calloc(1,sizeof(OBC_HashMap));

    if(map == NULL){
        return NULL;
    }

    if(OBC_initHashMapDepth(map,keySize, valueSize, bucketDepth) == NULL){
        free(map);
        return NULL;
    }

    return (void **)OBC_FROM_ALLOCFASTBIT_VAL(map->values);
}

void *OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize){
    return OBC_initHashMapDepth(map,keySize, valueSize,OBC_X_HASHMAP_DEFAULT_DEPTH);
}
void *OBC_initHashMapDepth(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t bucketDepth){

    if(OBC_initRay(& map->values, 0, valueSize ) == NULL){
        return NULL;
    }

    if(OBC_initRay(& map->keys, 0, keySize) == NULL){
        OBC_freeRayData(& map->values);
        return NULL;
    }

    if(OBC_initRay(& map->keyHashes, 0, sizeof(OBC_Hash)) == NULL){
        OBC_freeRayData(& map->values);
        OBC_freeRayData(& map->keys);
        return NULL;
    }


    map->buckets = 0;
    map->depth = 2;
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

    if(map->count == map->values.maxUnitLength){
        if(OBC_HashMapExpand(map) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    size_t pos = (hash&(map->buckets-1));

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






OBC_ERROR_ENUM OBC_HashMapExpand(OBC_HashMap *map){

    if(map->buckets == 0){

        const size_t mapCount = map->depth * OBC_X_HASHMAP_INIT_RESERVE_COUNT;

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

        return OBC_ERROR_SUCCESS;
    }

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

    ///*=2 but may not be for some reason
    map->buckets=map->values.maxUnitLength/map->depth;

    return OBC_ERROR_SUCCESS;
}


void OBC_HashMapNewIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    iter->iter = ( iter->hash & (map->buckets-1));
}
void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapNewIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}
void OBC_HashMapNewIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    ///pack keys together in access layers
    iter->iter+=map->buckets;
}
void OBC_HashMapNewIterNext(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapNewIterNextRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}
