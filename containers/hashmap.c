#include "hashmap.h"

void **OBC_newHashMap(size_t keySize, size_t valueSize){
    return OBC_newHashMapComplex(keySize, valueSize, 0, 1);
}
void **OBC_newHashMapComplex(size_t keySize, size_t valueSize, size_t initBucketCount, size_t initBucketSize){
    OBC_HashMap *map = calloc(1, sizeof(OBC_HashMap));

    if(map == NULL){
        return NULL;
    }

    if(OBC_initHashMapComplex(map, keySize, valueSize, initBucketCount, initBucketSize) == OBC_ERROR_FAILURE){
        free(map);
        return NULL;
    }

    return OBC_HashMapGetDataPointer(map);
}

OBC_ERROR_ENUM OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize){
    return OBC_initHashMapComplex(map, keySize, valueSize, 0, 1);
}

OBC_ERROR_ENUM OBC_initHashMapComplex(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t initBucketCount, size_t initBucketSize){

    size_t totalMem = initBucketCount*initBucketSize;

    if(OBC_initRayComplex(& map->values, totalMem, valueSize ) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& map->keys, totalMem, keySize) == OBC_ERROR_FAILURE){
        if(map->values.rawData != NULL){
            OBC_freeRayData(& map->values);
        }
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& map->keyHashes, totalMem, sizeof(OBC_Hash)) == OBC_ERROR_FAILURE){
        if(map->values.rawData != NULL){
            OBC_freeRayData(& map->values);
        }
        if(map->keys.rawData != NULL){
            OBC_freeRayData(& map->keys);
        }
        return OBC_ERROR_FAILURE;
    }

    map->itemsPerBucket = initBucketSize;
    map->buckets = initBucketCount;
    map->count = 0;
    map->depthBits = 1;
    map->listBits = 0;
    map->expandFlag = 0;

    return OBC_ERROR_SUCCESS;
}

void **OBC_HashMapGetDataPointer(OBC_HashMap *map){
    return (void **)OBC_FROM_RAY_VAL(map->values);
}
void **OBC_HashMapGetKeyPointer(void *arr){
    return OBC_HashMapGetKeyPointerRaw(OBC_TO_HASHMAP_PTR(arr));
}
void **OBC_HashMapGetKeyPointerRaw(OBC_HashMap *map){
    return (void **)OBC_FROM_RAY_VAL(map->keys);
}
void **OBC_HashMapGetHashPointer(void *arr){
    return OBC_HashMapGetHashPointerRaw(OBC_TO_HASHMAP_PTR(arr));
}
void **OBC_HashMapGetHashPointerRaw(OBC_HashMap *map){
    return (void **)OBC_FROM_RAY_VAL(map->keyHashes);
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

    if(OBC_initRayComplex(& map->values, mapCount, map->values.unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& map->keys, mapCount, map->keys.unitSize) == OBC_ERROR_FAILURE){
        if(map->values.rawData != NULL){
            OBC_freeRayData(& map->values);
        }
        return OBC_ERROR_FAILURE;
    }

    if(OBC_initRayComplex(& map->keyHashes, mapCount, map->keyHashes.unitSize) == OBC_ERROR_FAILURE){
        if(map->values.rawData != NULL){
            OBC_freeRayData(& map->values);
        }
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

    map->depthBits++;
    map->itemsPerBucket*=2;
    if(OBC_X_HashMapExpand(map)==OBC_ERROR_FAILURE){
        map->itemsPerBucket/=2;
        map->depthBits--;
        return OBC_ERROR_FAILURE;
    }
    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_HashMapExpandBucketCount(OBC_HashMap *map){

    map->listBits++;
    map->buckets*=2;
    if(OBC_X_HashMapExpand(map)==OBC_ERROR_FAILURE){
        map->buckets/=2;
        map->listBits--;
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

    OBC_Offset cuckooMask = map->itemsPerBucket-1;
    //OBC_Offset internal;

    ///FIXME DEBUG
    /**
    printf("BucketSize: %u\n",map->itemsPerBucket);
    //*/

    size_t i;
    for(i = 0; i < old_size; i++){

        if(hashes[offset-i] < OBC_X_HASHMAP_HASH_FREED){
            newOff = OBC_X_HASHMAP_HASH_TO_POSITION(hashes[offset-i],map) * map->itemsPerBucket;

            /*
            internal = ((hashes[offset-i]>>(map->listBits))&cuckooMask);

            if(hashes[newOff + internal] < OBC_X_HASHMAP_HASH_FREED){
                do{
                    internal = (internal+1)&cuckooMask;
                }while(hashes[newOff + internal] < OBC_X_HASHMAP_HASH_FREED);
            }

            newOff += internal;

            /*/

            /**if(newOff/2 != offset-i){
                continue;
            }*/

            //internal = 0;
            if(hashes[newOff] < OBC_X_HASHMAP_HASH_FREED){
                do{
                    //*
                    if(hashes[offset-i] == hashes[newOff/*+internal*/]){
                        //newOff = offset-i;
                        //puts("SELF BUCKET POS");
                        goto CONTINUE;
                    }
                    /*/
                    //*/
                    newOff++;
                    //internal++;
                    ///FIXME DEBUG
                    /**
                    if(internal >= map->itemsPerBucket){
                            puts("LINEAR_ADDRESS_OVERFLOW");
                            /*
                            OBC_HashMapExpandBucketSize(map);
                            /* /
                            OBC_HashMapExpandBucketCount(map);
                            //* /
                            return;
                        ///puts("LINEAR OPEN ADDRESSING");
                        //printf("IS OVERFLOW?? %u\n", (unsigned int)(OBC_X_HASHMAP_HASH_TO_POSITION(hashes[offset-i],map) * map->itemsPerBucket == (offset - i)) );
                        //printf("NEXT: %u    CUR_OFF: %u\n",(size_t)((OBC_X_HASHMAP_HASH_TO_POSITION(hashes[offset-i],map)+1) * map->itemsPerBucket),(size_t)(newOff));
                    }

                    //**/
                }while(hashes[newOff] < OBC_X_HASHMAP_HASH_FREED);

                //newOff+=internal;
            }

            //*/

            //if( (offset-i) == newOff ){
                //NO_ADD:continue;
            //}

            //printf("HASH: %u ;; at: %u\n",hashes[newOff], offset-i);
            hashes[newOff] = hashes[offset-i];
            hashes[offset-i] = OBC_X_HASHMAP_HASH_FREED;
            memcpy(keys + (keySize*newOff), keys + ((offset-i)*keySize), keySize);
            memcpy(values + (valueSize*newOff), values + ((offset-i)*valueSize), valueSize);
            //printf("NEW HASH: %u ;; at: %u\n",hashes[newOff], newOff);
        CONTINUE:;
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

    //map->expandFlag = 0;
    iter->keyCmpr = 1;
    iter->X_storage = OBC_NULL_INDEX;

    if(OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count){
        ///TODO change to expand allBuckets and expand internalBuckets
        //puts("ATTEMPTING TO EXPAND BUCKETS");
        if(OBC_HashMapExpandBucketCount(map) == OBC_ERROR_FAILURE){
            iter->iter = OBC_NULL_INDEX;
            ///TODO FIXME maybe allow fall through and only error on actual issue
            ///since unlss the bucket is full, most likely valid
            ///place to put data exists
            //return;
        }else{
            /*puts("EXPANDED BUCKET TOTAL");
            printf("BUCEKT COUNT: %u\n",map->buckets);
            printf("BUCEKTY SIZE: %u\n",map->itemsPerBucket);
            printf("MAX VALUES: %u\n",map->values.maxUnitLength);
            printf("MAX KEYS: %u\n",map->keyHashes.maxUnitLength);*/
        }
    }

    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash,map) * map->itemsPerBucket;
    iter->X_endIter = iter->iter + map->itemsPerBucket;
}

void OBC_HashMapSetIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}









void OBC_HashMapPutIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    if(iter->keyCmpr != 0){
        //*
        iter->iter = iter->X_endIter;
        iter->X_storage = OBC_NULL_INDEX;
        /*/
        iter->X_endIter = iter->iter;
        iter->X_storage = iter->iter;
        goto PUT;
        //*/
        return;
    }

    iter->iter++;
    if(iter->iter == iter->X_endIter){
        /*
        if(iter->X_storage == OBC_NULL_INDEX && ((OBC_Hash*)map->keyHashes.rawData)[iter->X_endIter-1] >= OBC_X_HASHMAP_HASH_EMPTY){
            iter->X_storage = iter->X_endIter-1;
        }
        */
        if(iter->X_storage == OBC_NULL_INDEX){
            if(map->expandFlag < 3 || OBC_X_HASHMAP_THRESHOLD_CALCULATION(map->values.maxUnitLength) <= map->count /*|| map->itemsPerBucket > 8*/){
                map->expandFlag++;
                if(OBC_HashMapExpandBucketCount(map) == OBC_ERROR_FAILURE){
                    iter->iter = OBC_NULL_INDEX;
                    iter->X_storage = OBC_NULL_INDEX;
                    //puts("FAILURE STORAGE EXPANSION");
                }else{
                    //puts("EXPANDED TABLE");
                    OBC_HashMapPutIterStartRaw(map,iter);
                }
            }else{
                /*
                unsigned int i;
                puts("ATTEMPT EXPANDED BUCKETS");
                printf("PRE ITEM COUNT: %u\n",map->count);
                printf("PRE BUCKET COUNT: %u\n",map->buckets);
                printf("PRE ITEMS/BUCKET COUNT: %u\n",map->itemsPerBucket);
                for(i = iter->X_endIter- map->itemsPerBucket; i < iter->X_endIter; i++){
                    printf(">> PRE ExHASH: %u\n",((OBC_Hash*)map->keyHashes.rawData)[i]);
                }
                */

                if(OBC_HashMapExpandBucketSize(map) == OBC_ERROR_FAILURE){
                    iter->X_storage = OBC_NULL_INDEX;
                }else{

                    /*
                    for(i = iter->X_endIter- map->itemsPerBucket; i < iter->X_endIter; i++){
                        printf(">> POST ExHASH: %u\n",((OBC_Hash*)map->keyHashes.rawData)[i]);
                    }
                    puts("EXPANDED BUCKETS");
                    printf("POST ITEM COUNT: %u\n",map->count);
                    printf("POST ITEMS/BUCKET COUNT: %u\n",map->itemsPerBucket);
                    printf("POST BUCKET COUNT: %u\n",map->buckets);
                    printf("MAX COUNT: %u\n",map->values.maxUnitLength);
                    */
                    OBC_HashMapPutIterStartRaw(map,iter);
                }
                map->expandFlag = 0;
            }
        }else{
            //printf("ADDED VALUE: %u\n",map->count);
            map->count++;
            ((OBC_Hash *)map->keyHashes.rawData)[iter->X_storage] = iter->hash;
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
    OBC_HashMapPutIterStartRaw(OBC_TO_HASHMAP_PTR(arr),iter);
}

void OBC_HashMapPutIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(map,iter);
    iter->keyCmpr = 0;
}








void OBC_HashMapGetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter){

    //printf("HASH: %u\n",((OBC_Hash *)map->keyHashes.rawData)[iter->iter]);
    //printf("|| CMPR= %i\n", iter->keyCmpr);
    if(iter->keyCmpr != 0){
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
    iter->keyCmpr = 0;
    iter->X_storage = OBC_NULL_INDEX;
    iter->iter = OBC_X_HASHMAP_HASH_TO_POSITION(iter->hash, map) * map->itemsPerBucket;
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
     OBC_HashMapNewIterNextRaw(OBC_TO_HASHMAP_PTR(arr), iter);
}

void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter){
    OBC_HashMapSetIterStartRaw(OBC_TO_HASHMAP_PTR(arr), iter);
}








OBC_Hash OBC_X_HashMapHashKeyRaw(OBC_HashMap *map, void *keyPtr){
    return OBC_hash3(keyPtr, map->keys.unitSize, OBC_X_HashMapLiteralBits(map->buckets) );
}

OBC_Hash OBC_X_HashMapHashKey(void *arr, void *keyPtr){
    return OBC_X_HashMapHashKeyRaw(OBC_TO_HASHMAP_PTR(arr), keyPtr);
}


unsigned int OBC_X_HashMapLowerBitCount(OBC_Offset value){

    if(value == 0){
        return 0;
    }

    unsigned char i;
    for(i = 1; i < CHAR_BIT*sizeof(OBC_Offset); i++){
        if( (value & (~((((size_t)1)<<i)-1))) == 0 ){
            break;
        }
    }
    if(i == CHAR_BIT*sizeof(OBC_Offset)){
        return 0;
    }

    return i;
}

unsigned int OBC_X_HashMapLiteralBits(OBC_Offset value){

    if(value == 0){
        return 0;
    }

    unsigned int i;
    for(i = 0; i < CHAR_BIT*sizeof(OBC_Offset); i++){
        if( (value & (((OBC_Offset)1)<<i)) != 0 ){
            break;
        }
    }

    if(i == CHAR_BIT*sizeof(OBC_Offset)){
        return CHAR_BIT*sizeof(OBC_Offset);
    }

    return i+1;
}

OBC_Hash OBC_HashMapFitHash(void *arr, OBC_Hash hash){

    OBC_HashMap *map = OBC_TO_HASHMAP_PTR(arr);
    return OBC_HashMapFitHashRaw(map, hash);
}

OBC_Hash OBC_HashMapFitHashRaw(OBC_HashMap *map, OBC_Hash hash){
    if(hash >= OBC_X_HASHMAP_HASH_FREED){
        hash-=3;
        ///puts(">>>>>       Sential Hash Collision");
    }
    //hash = (( (hash) & (map->buckets - 1)));
    return hash;
}

/*
OBC_Hash OBC_HashMapMakeHash(void *arr, void *data, size_t sizeInBytes){

    OBC_HashMap *map = OBC_TO_HASHMAP_PTR(arr);
    return OBC_HashMapFitHashRaw(map, data, sizeInBytes);
}
OBC_Hash OBC_HashMapMakeHashRaw(OBC_HashMap *map, void *data, size_t sizeInBytes){
    OBC_Hash hash = OBC_hash()
    (( (hash) & ((hashmapPtr)->buckets - 1)))
}
*/











































