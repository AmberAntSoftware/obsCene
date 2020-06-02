#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include "../obc.h"
#include "../allocators/alloclistbit.h"
#include "../obc_stdlib/hash.h"
#include "../obc_stdlib/memswap.h"
#include "../obc_stdlib/hash.h"

#define _OBC_HASHMAP_PTR_CAST(rawPtr) ((OBC_HashMap *)(rawPtr))
#define _OBC_HASHMAP_OFFSET ((size_t)(&((OBC_HashMap *)NULL)->values.rawData))
#define OBC_TO_HASHMAP_PTR(rawPtr) (_OBC_HASHMAP_PTR_CAST(((void*)(rawPtr)) - _OBC_HASHMAP_OFFSET))

#define OBC_FROM_HASHMAP_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_HASHMAP_OFFSET))
#define OBC_FROM_HASHMAP_VAL(hashmapVal) ((void**)(((void*)(&(hashmapVal))) + _OBC_HASHMAP_OFFSET))

#define OBC_X_HASHMAP_DEFAULT_MAX_DEPTH 8
#define OBC_X_HASHMAP_INIT_RESERVE_COUNT 16

#define OBC_X_HASHMAP_HASH_EMPTY (~((OBC_Offset)0))
#define OBC_X_HASHMAP_HASH_FREED (OBC_X_HASHMAP_HASH_EMPTY-1)

#define OBC_X_HASHMAP_HASH_TO_POSITION(hash, hashmapPtr) (( (hash) & ((hashmapPtr)->buckets - 1)))

//85% threshold before expanding
#define OBC_X_HASHMAP_THRESHOLD_CALCULATION(value) ((value)*85/100)

/**
SPEC

1 item per bucket on start

expand on new key when key position falls out of bounds of bucket -- no collision reduction needed

limit of 8 items per bucket max, eighth will have sparse list location in allocator

expand on threshold, bucket depth and buckets count

**/

typedef struct OBC_HashMap{

    ///the owned data
    OBC_Ray values;

    ///stored keys values for collision comparisons
    ///each bucket
    OBC_Ray keys;
    ///cached hashes of keys that should not change -- fast expansion
    OBC_Ray keyHashes;

    ///buckets is the N keys that can be stored in each layer
    OBC_Offset buckets;
    OBC_Offset itemsPerBucket;
    //if
    OBC_Offset rowMax;

    ///the living elements in this container
    ///maximum value at any given time is depth*buckets
    OBC_Offset count;

    unsigned char depthBits;
    unsigned char listBits;
    unsigned char expandFlag;

    //OBC_AllocFastBitCache sparseStorage;

}OBC_HashMap;

typedef struct OBC_HashMapIterator{

    OBC_Iterator iter;
    OBC_SpaceshipValue keyCmpr;

    OBC_Hash hash;
    void *key;
    void *value;

    OBC_Iterator X_endIter;
    OBC_Offset X_storage;

}OBC_HashMapIterator;

typedef struct OBC_HashMapPointer{

    OBC_Offset position;
    size_t X_state;

}OBC_HashMapPointer;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newHashMap(size_t keySize, size_t valueSize);
void **OBC_newHashMapComplex(size_t keySize, size_t valueSize, size_t initBucketCount, size_t initBucketSize);
OBC_ERROR_ENUM OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize);
OBC_ERROR_ENUM OBC_initHashMapComplex(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t initBucketCount, size_t initBucketSize);
void **OBC_HashMapGetDataPointer(OBC_HashMap *map);

void **OBC_HashMapGetKeyPointerRaw(OBC_HashMap *map);
void **OBC_HashMapGetKeyPointer(void *arr);

void **OBC_HashMapGetHashPointer(void *arr);
void **OBC_HashMapGetHashPointerRaw(OBC_HashMap *map);

void OBC_freeHashMap(void *arr);
void OBC_freeHashMapData(OBC_HashMap *map);

OBC_Offset OBC_Hashmap_count(void *arr);

/*
OBC_Offset OBC_HashMapNewRaw(OBC_HashMap *map, void *key, OBC_Hash hash);
OBC_Offset OBC_HashMapNew(void *arr, void *key, OBC_Hash hash);

OBC_Offset OBC_HashMapNewSimpleRaw(OBC_HashMap *map, void *key);
OBC_Offset OBC_HashMapNewSimple(void *arr, void *key);

OBC_Hash OBC_HashMapHashRaw(OBC_HashMap *map, void *clearedPaddingData);
OBC_Hash OBC_HashMapHash(void *arr, void *clearedPaddingData);*/

#define OBC_HashMapSetLoop(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr) \
OBC___X___HASHMAP_ITER_LOOP_TEMPLATE(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr, OBC_HashMapSetIterStart, OBC_HashMapSetIterNext)

#define OBC_HashMapPutLoop(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr) \
OBC___X___HASHMAP_ITER_LOOP_TEMPLATE(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr, OBC_HashMapPutIterStart, OBC_HashMapPutIterNext)

#define OBC_HashMapGetLoop(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr) \
OBC___X___HASHMAP_ITER_LOOP_TEMPLATE(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr, OBC_HashMapGetIterStart, OBC_HashMapGetIterNext)


/************************************************/
/***************CURRENT_REAL_INTERFACES**********************/
/************************************************/

void OBC_HashMapSetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapSetIterNext(void *arr, OBC_HashMapIterator *iter);
void OBC_HashMapSetIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapSetIterStart(void *arr, OBC_HashMapIterator *iter);

void OBC_HashMapPutIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapPutIterNext(void *arr, OBC_HashMapIterator *iter);

void OBC_HashMapPutIterStart(void *arr, OBC_HashMapIterator *iter);
void OBC_HashMapPutIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);

OBC_ERROR_ENUM OBC_HashMapExpandBucketSize(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapExpandBucketCount(OBC_HashMap *map);

OBC_ERROR_ENUM OBC_X_HashMapExpand(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_X_HashMapHowExpand(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_X_HashMapExpandEmpty(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_X_HashMapExpandNonEmpty(OBC_HashMap *map);

void OBC_HashMapGetIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapGetIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapGetIterNext(void *arr, OBC_HashMapIterator *iter);
void OBC_HashMapGetIterStart(void *arr, OBC_HashMapIterator *iter);

/************************************************/
/************************************************************/
/************************************************/



#define OBC___X___HASHMAP_ITER_LOOP_TEMPLATE(arrPtr, HashMapIteratorPtr, keyPtr, keyHash, valuePtr, START_FUNC, INCREMENT_FUNC) \
(HashMapIteratorPtr)->hash = /**keyHash;/**/ /**/OBC_HashMapFitHash(arrPtr,keyHash);/**/ \
(HashMapIteratorPtr)->key = keyPtr; \
(HashMapIteratorPtr)->value = valuePtr; \
for( START_FUNC(arrPtr, (HashMapIteratorPtr)); (HashMapIteratorPtr)->iter < (HashMapIteratorPtr)->X_endIter && (HashMapIteratorPtr)->iter <= OBC_X_HASHMAP_HASH_FREED; INCREMENT_FUNC(arrPtr, (HashMapIteratorPtr)) ) \
if( (((OBC_Hash *)OBC_TO_HASHMAP_PTR(arrPtr)->keyHashes.rawData)[(HashMapIteratorPtr)->iter] |1) == OBC_X_HASHMAP_HASH_EMPTY){ \
(HashMapIteratorPtr)->X_storage = (HashMapIteratorPtr)->iter; \
/**puts("CONTINUE, iter hash is EMPTY Node");/**/\
continue;\
}else if( ((OBC_Hash *)OBC_TO_HASHMAP_PTR(arrPtr)->keyHashes.rawData)[(HashMapIteratorPtr)->iter] == (HashMapIteratorPtr)->hash )




#define OBC___X___HASHMAP_ITER_LOOP_TEMPLATE_MIN(arrPtr, HashMapIteratorPtr, keyPtr, valuePtr, START_FUNC, INCREMENT_FUNC) \
(HashMapIteratorPtr)->hash = /**keyHash;/**/ /**/OBC_HashMapFitHash(arrPtr,keyHash);/**/ \
(HashMapIteratorPtr)->key = keyPtr; \
(HashMapIteratorPtr)->value = valuePtr; \
for( START_FUNC(arrPtr, (HashMapIteratorPtr)); (HashMapIteratorPtr)->iter < (HashMapIteratorPtr)->X_endIter && (HashMapIteratorPtr)->iter <= OBC_X_HASHMAP_HASH_FREED; INCREMENT_FUNC(arrPtr, (HashMapIteratorPtr)) ) \
if( (((OBC_Hash *)OBC_TO_HASHMAP_PTR(arrPtr)->keyHashes.rawData)[(HashMapIteratorPtr)->iter] |1) == OBC_X_HASHMAP_HASH_EMPTY){ \
(HashMapIteratorPtr)->X_storage = (HashMapIteratorPtr)->iter; \
/**puts("CONTINUE, iter hash is EMPTY Node");/**/\
continue;\
}else if( ((OBC_Hash *)OBC_TO_HASHMAP_PTR(arrPtr)->keyHashes.rawData)[(HashMapIteratorPtr)->iter] == (HashMapIteratorPtr)->hash )



OBC_Hash OBC_X_HashMapHashKey(void *arr, void *keyPtr);
OBC_Hash OBC_X_HashMapHashKeyRaw(OBC_HashMap *map, void *keyPtr);
unsigned int OBC_X_HashMapLiteralBits(OBC_Offset value);






///TODO add in new helper methods for  hashmap funnctions

///TODO FIXME "template" inline helper functions for 'functionless' comparator func pointer add methods
#define OBC_X_HashMapCheckKeyLoop() {}
//if no space or exists, return null
//check the last bucket position if it is a non ideal hash tag
//if it is used, then check key and expand map if key does not exist due to it being in cache still, or
//cold if it does not exist, could use a second iterator or return a complex structure to check location
typedef struct OBC_HashMapKeyCheck{

    OBC_ERROR_ENUM err;
    OBC_Offset position;

}OBC_HashMapKeyCheck;


OBC_Hash OBC_HashMapHashAdd(void *arr, void *data, void *key, OBC_Hash hash, OBC_SpaceshipValue (*comparator)(void *val1, void *val2));

#define OBC_HashMapAddLoop(arrPtr, iterPtr, spaceshipValue) for(iter = OBC_TreeIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNext(arrPtr, iter))



//#define OBC_HashMapSetLoop(arrPtr, iterPtr) for(OBC_HashMapSetIterStart(arrPtr); (iterPtr)->X_endIter != OBC_NULL_INDEX; OBC_HashMapSetIterNext(arrPtr, iterPtr))




void OBC_HashMapIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapIterStart(void *arr, OBC_HashMapIterator *iter);

#define OBC_HashMapNewLoop(arrPtr, HashMapIterator)
/**
if( OBC_X_HASHMAP_HASH_TO_POSITION(OBC_FROM_HASHMAP_PTR(arrPtr)) ) == OBC_X_HASHMAP_HASH_FREED)
()
//*/

///internal utility
OBC_ERROR_ENUM OBC_HashMapDoExpand(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapExpand(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapExpandEmpty(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapExpandNonEmpty(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapExpandSteppes(OBC_HashMap *map);
OBC_ERROR_ENUM OBC_HashMapAlignSteppes(OBC_HashMap *map);

void OBC_HashMapNewIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterNext(void *arr, OBC_HashMapIterator *iter);


OBC_Hash OBC_HashMapFitHash(void *arr, OBC_Hash hash);
OBC_Hash OBC_HashMapFitHashRaw(OBC_HashMap *map, OBC_Hash hash);











typedef struct OBC_OOPHashMap{

    OBC_HashMap map;

    void (*freeKeyResources)(void *key);
    void (*freeValueResources)(void *value);
    OBC_SpaceshipValue (*keyComparatorFunc)(void *checkedKey, void *storedKey);

}OBC_OOPHashMap;



typedef struct OBC_HashMapStatic{

    OBC_AllocListBit backed;
    OBC_HashMap indirection;

}OBC_HashMapStatic;

#endif // HASHMAP_H_INCLUDED
