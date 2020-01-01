#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include "OBC.h"
#include "allocfastbit.h"
#include "hash.h"

#define _OBC_HASHMAP_PTR_CAST(rawPtr) ((OBC_HashMap *)(rawPtr))
#define _OBC_HASHMAP_OFFSET ((size_t)(&((OBC_HashMap *)NULL)->values.backed.rawData))
#define OBC_TO_HASHMAP_PTR(rawPtr) (_OBC_HASHMAP_PTR_CAST(((void*)(rawPtr)) - _OBC_HASHMAP_OFFSET))

#define OBC_FROM_HASHMAP_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_HASHMAP_OFFSET))
#define OBC_FROM_HASHMAP_VAL(hashmapVal) ((void**)(((void*)(&(hashmapVal))) + _OBC_HASHMAP_OFFSET))

#define OBC_X_HASHMAP_DEFAULT_DEPTH 8

typedef struct OBC_HashMap{

    ///the owned data
    OBC_AllocFastBit values;

    ///cached keys into values (pointer list) for what item is stored in
    ///each bucket
    OBC_Ray keyValues;

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

void **OBC_newHashMap(size_t keySize, size_t valueSize);
void **OBC_newHashMapDepth(size_t keySize, size_t valueSize, size_t bucketDepth);
void *OBC_initHashMap(OBC_HashMap *map, size_t keySize, size_t valueSize);
void *OBC_initHashMapDepth(OBC_HashMap *map, size_t keySize, size_t valueSize, size_t bucketDepth);
void **OBC_HashMapGetAccessPointer(OBC_HashMap *map);

void OBC_freeHashMap(void *arr);
void OBC_freeHashMapData(OBC_HashMap *map);

#endif // HASHMAP_H_INCLUDED
