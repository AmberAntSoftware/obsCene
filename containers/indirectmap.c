#include "indirectmap.h"

void **OBC_newIndirectMap(size_t keySize, size_t valueSize){
    return OBC_newIndirectMapComplex(keySize, valueSize, 0, 1, OBC_X_HASHMAP_DEFAULT_THRESHOLD);
}

void **OBC_newIndirectMapThreshold(size_t keySize, size_t valueSize, unsigned char threshold){
    return OBC_newIndirectMapComplex(keySize, valueSize, 0, 1, threshold);
}

void **OBC_newIndirectMapComplex(size_t keySize, size_t valueSize, OBC_Offset initBucketCount, OBC_Offset initBucketSize, unsigned char threshold){
    OBC_IndirectMap *map = calloc(1, sizeof(OBC_IndirectMap));

    if(map == NULL){
        return NULL;
    }

    if(OBC_initIndirectMapComplex(map, keySize, valueSize, initBucketCount, initBucketSize, threshold) == OBC_ERROR_FAILURE){
        free(map);
        return NULL;
    }

    return OBC_IndirectMapGetDataPointer(map);
}

OBC_ERROR_ENUM OBC_initIndirectMap(OBC_IndirectMap *map, size_t keySize, size_t valueSize){
    return OBC_initIndirectMapComplex(map, keySize, valueSize, 0, 1, OBC_X_HASHMAP_DEFAULT_THRESHOLD);
}

OBC_ERROR_ENUM OBC_initIndirectMapThreshold(OBC_IndirectMap *map, size_t keySize, size_t valueSize, unsigned char threshold){
    return OBC_initIndirectMapComplex(map, keySize, valueSize, 0, 1, threshold);
}

OBC_ERROR_ENUM OBC_initIndirectMapComplex(OBC_IndirectMap *map, size_t keySize, size_t valueSize, OBC_Offset initBucketCount, OBC_Offset initBucketSize, unsigned char threshold){

    if(OBC_initHashMapComplex(& map->indirection, sizeof(OBC_Offset), 0, initBucketCount, initBucketSize, threshold) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initAllocListBit(& map->values, valueSize) == OBC_ERROR_FAILURE){
        OBC_freeHashMapData(& map->indirection);
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initAllocListBit(& map->keys, keySize) == OBC_ERROR_FAILURE){
        OBC_freeHashMapData(& map->indirection);
        OBC_freeAllocListBitData(& map->values);
        return OBC_ERROR_FAILURE;
    }


    return OBC_ERROR_SUCCESS;
}

void **OBC_IndirectMapGetDataPointer(OBC_IndirectMap *map){
    return OBC_AllocListBitGetDataPointer(& map->values);
}

void **OBC_IndirectMapGetKeyPtr(void *arr){
    return OBC_IndirectMapGetKeyPtrRaw(OBC_TO_INDIRECTMAP_PTR(arr));
}
void **OBC_IndirectMapGetKeyPtrRaw(OBC_IndirectMap *map){
    return OBC_AllocListBitGetDataPointer(& map->keys);
}

OBC_Hash **OBC_IndirectMapGetHashPtr(void *arr){
    return OBC_IndirectMapGetHashPtrRaw(OBC_TO_INDIRECTMAP_PTR(arr));
}
OBC_Hash **OBC_IndirectMapGetHashPtrRaw(OBC_IndirectMap *map){
    return OBC_HashMapGetHashPointerRaw(& map->indirection);
}


void OBC_freeIndirectMap(void *arr){

    OBC_IndirectMap *map = OBC_TO_INDIRECTMAP_PTR(arr);
    OBC_freeIndirectMapData(map);
    free(map);
}
void OBC_freeIndirectMapData(OBC_IndirectMap *map){

    OBC_freeHashMapData(& map->indirection);
    OBC_freeAllocListBitData(& map->values);
    OBC_freeAllocListBitData(& map->keys);
}

OBC_Offset **OBC_X_IndirectMapIndirects(void *arr){
    return OBC_X_IndirectMapIndirectsRaw(OBC_TO_INDIRECTMAP_PTR(arr));
}

OBC_Offset **OBC_X_IndirectMapIndirectsRaw(OBC_IndirectMap *map){
    return (OBC_Offset **)OBC_HashMapGetKeyPointerRaw(& map->indirection);
}



void OBC_IndirectMapPutIterStart(void *arr, OBC_IndirectMapIterator *iter){
    OBC_IndirectMapPutIterStartRaw(OBC_TO_INDIRECTMAP_PTR(arr), iter);
}
void OBC_IndirectMapPutIterStartRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter){

    OBC_Offset store1 = OBC_AllocListBitMallocRaw(& map->keys);
    if(store1 == OBC_NULL_INDEX){
        iter->X_mapIter.iter = OBC_NULL_INDEX;
        iter->X_mapIter.X_endIter = OBC_NULL_INDEX;
        return;
    }
    OBC_Offset store2 = OBC_AllocListBitMallocRaw(& map->values);
    if(store2 == OBC_NULL_INDEX){
        OBC_AllocListBitFreeRaw(& map->keys, store1);
        iter->X_mapIter.iter = OBC_NULL_INDEX;
        iter->X_mapIter.X_endIter = OBC_NULL_INDEX;
        return;
    }
    if(store1 != store2){
        OBC_AllocListBitFreeRaw(& map->keys, store1);
        OBC_AllocListBitFreeRaw(& map->values, store2);
        iter->X_mapIter.iter = OBC_NULL_INDEX;
        iter->X_mapIter.X_endIter = OBC_NULL_INDEX;
        return;
    }

    OBC_HashMapPutIterStartRaw(& map->indirection, & iter->X_mapIter);

    iter->indirectKey = store1;
    iter->key = iter->X_mapIter.key;
    iter->X_mapIter.key = &iter->indirectKey;
    iter->keyCmpr = 0;
    iter->X_mapIter.keyCmpr = 0;

    /*if(store1 > 3000000){
        puts("3mil");
        printf("BUCKET COUNT: %u     ITEMS: %u\n", map->indirection.itemsPerBucket, map->indirection.count);
    }*/

    ///
    if(iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED){
        iter->iter = iter->X_locs[obc(iter->X_mapIter.iter)];
    }
}


void OBC_IndirectMapPutIterNext(void *arr, OBC_IndirectMapIterator *iter){
    OBC_IndirectMapPutIterNextRaw(OBC_TO_INDIRECTMAP_PTR(arr), iter);
}
void OBC_IndirectMapPutIterNextRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter){

    iter->X_mapIter.keyCmpr = iter->keyCmpr;
    /*
    if(iter->keyCmpr != 0 || iter->X_mapIter.iter+1 == iter->X_mapIter.X_endIter){
        ///printf("CMPR: %u     STORAGE: %u\n", iter->keyCmpr, iter->X_mapIter.X_storage);
    }
    /*/
    if(iter->keyCmpr != 0){
        OBC_AllocListBitFreeRaw(& map->keys, iter->indirectKey);
        OBC_AllocListBitFreeRaw(& map->values, iter->indirectKey);
    }
    //*/

    OBC_HashMapPutIterNextRaw(& map->indirection, & iter->X_mapIter);

    if(iter->X_mapIter.iter == iter->X_mapIter.X_endIter && iter->X_mapIter.X_storage != OBC_NULL_INDEX){
        memcpy(map->keys.backed.rawData + (map->keys.backed.unitSize * iter->indirectKey), iter->key, map->keys.backed.unitSize);
        memcpy(map->values.backed.rawData + (map->values.backed.unitSize * iter->indirectKey), iter->X_mapIter.value, map->values.backed.unitSize);
        //memcpy(map->values.rawData + (map->values.unitSize * iter->X_storage),iter->value, map->values.unitSize);

    }

    /*
    if(iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED){
        iter->iter = iter->X_locs[obc(iter->X_mapIter.iter)];
    }
    */
    //iter->iter = (iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED) ? iter->X_locs[obc(iter->X_mapIter.iter)] : OBC_NULL_INDEX;
    ///(indirectionMapIteratorPtr)->iter = ((indirectionMapIteratorPtr)->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED) ? (indirectionMapIteratorPtr)->X_locs[obc((indirectionMapIteratorPtr)->X_mapIter.iter)] : OBC_NULL_INDEX;
}












void OBC_IndirectMapGetIterNextRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter){

    iter->X_mapIter.keyCmpr = iter->keyCmpr;

    /*if(iter->keyCmpr != 0 || iter->X_mapIter.iter >= OBC_X_HASHMAP_HASH_FREED){
        iter->X_mapIter.X_endIter = iter->X_mapIter.iter;
        iter->iter = iter->X_mapIter.iter;
        return;
    }*/

    //(indirectionMapIteratorPtr)->iter = ((indirectionMapIteratorPtr)->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED) ? (indirectionMapIteratorPtr)->X_locs[obc((indirectionMapIteratorPtr)->X_mapIter.iter)] : OBC_NULL_INDEX )

    OBC_HashMapGetIterNextRaw(& map->indirection, & iter->X_mapIter);

    if(iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED){
        iter->iter = iter->X_locs[obc(iter->X_mapIter.iter)];
    }
//if(iter->key == 0)
//    iter->iter = (iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED) ? iter->X_locs[obc(iter->X_mapIter.iter)] : OBC_NULL_INDEX;

    //iter->keyCmpr = 0;
    //iter->X_mapIter.keyCmpr = 0;
}


void OBC_IndirectMapGetIterStartRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter){

    iter->keyCmpr = 0;

    OBC_HashMapGetIterStartRaw(& map->indirection, & iter->X_mapIter);

    if(iter->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED){
        iter->iter = iter->X_locs[obc(iter->X_mapIter.iter)];
    }

}

void OBC_IndirectMapGetIterNext(void *arr, OBC_IndirectMapIterator *iter){
     OBC_IndirectMapGetIterNextRaw(OBC_TO_INDIRECTMAP_PTR(arr),iter);
}

void OBC_IndirectMapGetIterStart(void *arr, OBC_IndirectMapIterator *iter){
    OBC_IndirectMapGetIterStartRaw(OBC_TO_INDIRECTMAP_PTR(arr),iter);
}
