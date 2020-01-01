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

    /**
    if(OBC_initRay(& map->keyValues,0,sizeof(size_t)*OBC_TREE_LINK_COUNT) == NULL){
        return NULL;
    }

    if(OBC_initRay(& tree->links,0,sizeof(size_t)*OBC_TREE_LINK_COUNT) == NULL){
        return NULL;
    }

    if(OBC_initAllocFastBit(& tree->allocator, unitSize ) == NULL){
        OBC_freeRayData(& tree->links);
        return NULL;
    }

    tree->root = OBC_NULL_INDEX;
    **/

    return map;
}

void **OBC_HashMapGetAccessPointer(OBC_HashMap *map);

void OBC_freeHashMap(void *arr);
void OBC_freeHashMapData(OBC_HashMap *map);
