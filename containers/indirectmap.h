#ifndef INDIRECTMAP_H_INCLUDED
#define INDIRECTMAP_H_INCLUDED

#include "../allocators/alloclistbit.h"
#include "../containers/hashmap.h"
#include "../obc_stdlib/hash.h"

typedef struct OBC_IndirectMap{

    OBC_AllocListBit values;
    OBC_AllocListBit keys;

    OBC_HashMap indirection;

}OBC_IndirectMap;

typedef struct OBC_IndirectMapIterator{

    OBC_Offset iter;
    OBC_SpaceshipValue keyCmpr;

    OBC_Offset indirectKey;
    void *key;

    OBC_HashMapIterator X_mapIter;
    OBC_Offset **X_locs;
    void **rawMap;

}OBC_IndirectMapIterator;

/*************************************
Initialization
*************************************/

void **OBC_newIndirectMap(size_t keySize, size_t valueSize);
void **OBC_newIndirectMapThreshold(size_t keySize, size_t valueSize, unsigned char threshold);
void **OBC_newIndirectMapComplex(size_t keySize, size_t valueSize, OBC_Offset initBucketCount, OBC_Offset initBucketSize, unsigned char threshold);

OBC_ERROR_ENUM OBC_initIndirectMap(OBC_IndirectMap *map, size_t keySize, size_t valueSize);
OBC_ERROR_ENUM OBC_initIndirectMapThreshold(OBC_IndirectMap *map, size_t keySize, size_t valueSize, unsigned char threshold);
OBC_ERROR_ENUM OBC_initIndirectMapComplex(OBC_IndirectMap *map, size_t keySize, size_t valueSize, OBC_Offset initBucketCount, OBC_Offset initBucketSize, unsigned char threshold);

/*************************************
Deallocation
*************************************/

void OBC_freeIndirectMap(void *arr);

void OBC_freeIndirectMapData(OBC_IndirectMap *map);

/*************************************
Data Accessors
*************************************/

void **OBC_IndirectMapGetDataPointer(OBC_IndirectMap *map);

void **OBC_IndirectMapGetKeyPtr(void *arr);
void **OBC_IndirectMapGetKeyPtrRaw(OBC_IndirectMap *map);

OBC_Hash **OBC_IndirectMapGetHashPtr(void *arr);
OBC_Hash **OBC_IndirectMapGetHashPtrRaw(OBC_IndirectMap *map);

#define OBC_IndirectMapMapGetLoop__(arrPtr, hashMapIteratorPtr, keyHash) \
(indirectionMapIteratorPtr)->rawMap = OBC_HashMapGetDataPointer(& (OBC_TO_INDIRECTMAP_PTR(arrPtr)->indirection)); \
(indirectionMapIteratorPtr)->iter = OBC_NULL_INDEX; \
(indirectionMapIteratorPtr)->X_locs = OBC_X_IndirectMapIndirects(arrPtr); \
OBC_X_HASHMAP_ITER_INIT(arrPtr, (& (indirectionMapIteratorPtr)->X_mapIter), NULL, keyHash, NULL) \
for( OBC_HashMapGetIterStart(arrPtr, hashMapIteratorPtr); \
    (hashMapIteratorPtr)->iter < (hashMapIteratorPtr)->X_endIter && (hashMapIteratorPtr)->iter < OBC_X_HASHMAP_HASH_FREED; \
    OBC_HashMapGetIterNext(arrPtr, (hashMapIteratorPtr)) ) \
OBC_X_HASHMAP_LOOP_END(arrPtr, hashMapIteratorPtr, keyPtr, keyHash, NULL)


#define OBC_IndirectMapPutLoop__(arrPtr, indirectionMapIteratorPtr, keyPtr, keyHash, valuePtr) \
(indirectionMapIteratorPtr)->rawMap = OBC_HashMapGetDataPointer(& (OBC_TO_INDIRECTMAP_PTR(arrPtr)->indirection)); \
(indirectionMapIteratorPtr)->iter = OBC_NULL_INDEX; \
(indirectionMapIteratorPtr)->X_locs = OBC_X_IndirectMapIndirects(arrPtr); \
OBC_X_HASHMAP_ITER_INIT(arrPtr, (& (indirectionMapIteratorPtr)->X_mapIter), keyPtr, keyHash, valuePtr) \
for( OBC_IndirectMapPutIterStart(arrPtr, indirectionMapIteratorPtr); \
    (indirectionMapIteratorPtr)->X_mapIter.iter < (indirectionMapIteratorPtr)->X_mapIter.X_endIter && (indirectionMapIteratorPtr)->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED; \
    OBC_IndirectMapPutIterNext(arrPtr, indirectionMapIteratorPtr), (indirectionMapIteratorPtr)->iter = ((indirectionMapIteratorPtr)->X_mapIter.iter < OBC_X_HASHMAP_HASH_FREED) ? (indirectionMapIteratorPtr)->X_locs[obc((indirectionMapIteratorPtr)->X_mapIter.iter)] : OBC_NULL_INDEX \
) \
/**/OBC_X_HASHMAP_LOOP_END((indirectionMapIteratorPtr)->rawMap, (&(indirectionMapIteratorPtr)->X_mapIter), keyPtr, keyHash, valuePtr) /**/
/** \
if( (((OBC_Hash *)OBC_TO_INDIRECTMAP_PTR(arrPtr)->indirection.keyHashes.rawData)[(indirectionMapIteratorPtr)->X_mapIter.iter]) >= OBC_X_HASHMAP_HASH_FREED){ \
    if((indirectionMapIteratorPtr)->X_mapIter.X_storage == OBC_NULL_INDEX){(indirectionMapIteratorPtr)->X_mapIter.X_storage = (indirectionMapIteratorPtr)->X_mapIter.iter;}\
    if( (((OBC_Hash *)OBC_TO_INDIRECTMAP_PTR(arrPtr)->indirection.keyHashes.rawData)[(indirectionMapIteratorPtr)->X_mapIter.iter]) == OBC_X_HASHMAP_HASH_EMPTY){\
        (indirectionMapIteratorPtr)->X_mapIter.iter = OBC_NULL_INDEX-1;\
        (indirectionMapIteratorPtr)->X_mapIter.X_endIter = OBC_NULL_INDEX;\
    } \
    continue;\
}else if( (((OBC_Hash *)OBC_TO_INDIRECTMAP_PTR(arrPtr)->indirection.keyHashes.rawData)[(indirectionMapIteratorPtr)->X_mapIter.iter]) != (indirectionMapIteratorPtr)->X_mapIter.hash ){ \
continue; \
}else
//*/
//OBC_IndirectMap.indirection
//OBC_HashMapGetDataPointer(& (OBC_TO_INDIRECTMAP_PTR(NULL)->indirection))

/*************************************
Internal Utilities
*************************************/

#define _OBC_INDIRECTMAP_PTR_CAST(rawPtr) ((OBC_IndirectMap *)(rawPtr))
#define _OBC_INDIRECTMAP_OFFSET ((size_t)(&((OBC_IndirectMap *)NULL)->values.backed.rawData))
#define OBC_TO_INDIRECTMAP_PTR(rawPtr) (_OBC_INDIRECTMAP_PTR_CAST(((void*)(rawPtr)) - _OBC_INDIRECTMAP_OFFSET))

#define OBC_FROM_INDIRECTMAP_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_INDIRECTMAP_OFFSET))
#define OBC_FROM_INDIRECTMAP_VAL(indirectmapVal) ((void**)(((void*)(&(indirectmapVal))) + _OBC_INDIRECTMAP_OFFSET))

OBC_Offset **OBC_X_IndirectMapIndirects(void *arr);
OBC_Offset **OBC_X_IndirectMapIndirectsRaw(OBC_IndirectMap *map);

void OBC_IndirectMapPutIterNext(void *arr, OBC_IndirectMapIterator *iter);
void OBC_IndirectMapPutIterNextRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter);

void OBC_IndirectMapPutIterStart(void *arr, OBC_IndirectMapIterator *iter);
void OBC_IndirectMapPutIterStartRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter);

void OBC_IndirectMapGetIterNextRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter);
void OBC_IndirectMapGetIterStartRaw(OBC_IndirectMap *map, OBC_IndirectMapIterator *iter);
void OBC_IndirectMapGetIterNext(void *arr, OBC_IndirectMapIterator *iter);
void OBC_IndirectMapGetIterStart(void *arr, OBC_IndirectMapIterator *iter);

#endif // INDIRECTMAP_H_INCLUDED
