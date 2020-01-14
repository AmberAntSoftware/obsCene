#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include "OBC.h"
#include "allocfastbit.h"
#include "hash.h"
#include "memswap.h"

#define _OBC_HASHMAP_PTR_CAST(rawPtr) ((OBC_HashMap *)(rawPtr))
#define _OBC_HASHMAP_OFFSET ((size_t)(&((OBC_HashMap *)NULL)->values.rawData))
#define OBC_TO_HASHMAP_PTR(rawPtr) (_OBC_HASHMAP_PTR_CAST(((void*)(rawPtr)) - _OBC_HASHMAP_OFFSET))

#define OBC_FROM_HASHMAP_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_HASHMAP_OFFSET))
#define OBC_FROM_HASHMAP_VAL(hashmapVal) ((void**)(((void*)(&(hashmapVal))) + _OBC_HASHMAP_OFFSET))

#define OBC_X_HASHMAP_DEFAULT_DEPTH 2
#define OBC_X_HASHMAP_INIT_RESERVE_COUNT 16

#define OBC_X_HASHMAP_HASH_EMPTY (~((OBC_Offset)0))
#define OBC_X_HASHMAP_HASH_FREED ((~((OBC_Offset)0))-1)

typedef struct OBC_HashMap{

    ///the owned data
    OBC_Ray values;

    ///stored keys values for collision comparisons
    ///each bucket
    OBC_Ray keys;
    ///cached hashes of keys that should not change -- fast expansion
    OBC_Ray keyHashes;

    ///buckets is the N keys that can be stored
    size_t buckets;
    ///depth is the number of units that can be stored in each bucket
    ///before expansion -- if one bucket overflows, the entire table
    ///will be reexpanded
    size_t depth;

    ///the living elements in this container
    ///maximum value at any given time is depth*buckets
    size_t count;

}OBC_HashMap;

typedef struct OBC_HashMapIterator{

    OBC_Iterator iter;
    OBC_Hash hash;
    OBC_SpaceshipValue keyCmpr;

}OBC_HashMapIterator;

void **OBC_newHashMap(size_t keySize, size_t valueSize);
void **OBC_newHashMapDepth(size_t keySize, size_t valueSize, size_t bucketDepth);
void *OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize);
void *OBC_initHashMapDepth(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t bucketDepth);
void **OBC_HashMapGetDataPointer(OBC_HashMap *map);

void **OBC_HashMapGetKeyPointerRaw(OBC_HashMap *map);
void **OBC_HashMapGetKeyPointer(void *arr);

void OBC_freeHashMap(void *arr);
void OBC_freeHashMapData(OBC_HashMap *map);

OBC_Offset OBC_HashMapNewRaw(OBC_HashMap *map, void *key, OBC_Hash hash);
OBC_Offset OBC_HashMapNew(void *arr, void *key, OBC_Hash hash);

OBC_Offset OBC_HashMapNewSimpleRaw(OBC_HashMap *map, void *key);
OBC_Offset OBC_HashMapNewSimple(void *arr, void *key);

OBC_Hash OBC_HashMapHashRaw(OBC_HashMap *map, void *clearedPaddingData);
OBC_Hash OBC_HashMapHash(void *arr, void *clearedPaddingData);

#define OBC_HashMapAddLoop(arrPtr, iter, spaceshipValue) for(iter = OBC_TreeIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNext(arrPtr, iter))

#define OBC_HashMapNewLoop(arrPtr, HashMapIterator)
/*if( ((keyHash)&(map->buckets-1)) == OBC_X_HASHMAP_HASH_FREED)
()*/

///internal utility
OBC_ERROR_ENUM OBC_HashMapExpand(OBC_HashMap *map);

void OBC_HashMapNewIterStartRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterStart(void *arr, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterNextRaw(OBC_HashMap *map, OBC_HashMapIterator *iter);
void OBC_HashMapNewIterNext(void *arr, OBC_HashMapIterator *iter);

















typedef struct OBC_HashMapStatic{

    ///the owned data
    OBC_AllocFastBit values;

    ///Indexed by hash, points to values and keys + keyhashes, sparse, better memory when unfilled hashtable
    OBC_Ray indirectionList;

    ///stored keys values for collision comparisons
    ///each bucket
    OBC_Ray keys;
    ///cached hashes of keys that should not change -- fast expansion
    OBC_Ray keyHashes;

    ///buckets is the N keys that can be stored
    size_t buckets;
    ///depth is the number of units that can be stored in each bucket
    ///before expansion -- if one bucket overflows, the entire table
    ///will be reexpanded
    size_t depth;

    ///the living elements in this container
    ///maximum value at any given time is depth*buckets
    size_t count;

}OBC_HashMapStatic;

#endif // HASHMAP_H_INCLUDED
