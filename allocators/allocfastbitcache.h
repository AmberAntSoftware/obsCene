#ifndef ALLOCFASTBITCACHE_H_INCLUDED
#define ALLOCFASTBITCACHE_H_INCLUDED

#include "../containers/ray.h"

#define _OBC_ALLOCFASTBITCACHE_PTR_CAST(rawPtr) ((OBC_AllocFastBitCache *)(rawPtr))
#define _OBC_ALLOCFASTBITCACHE_OFFSET ((size_t)(&((OBC_AllocFastBitCache *)NULL)->backed.rawData))
#define OBC_TO_RAW_ALLOCFASTBITCACHE(rawPtr) (_OBC_ALLOCFASTBITCACHE_PTR_CAST(((void*)(rawPtr)) - _OBC_ALLOCFASTBITCACHE_OFFSET))

#define OBC_ALLOCFASTBITCACHE_META unsigned int
#define OBC_ALLOCFASTBITCACHE_ADDR unsigned int

#define OBC_ALLOCFASTBITCACHE_META_FULL ((OBC_ALLOCFASTBITCACHE_META)(~((OBC_ALLOCFASTBITCACHE_META)0)))
#define OBC_ALLOCFASTBITCACHE_STACK_SIZE 0

#define OBC_FROM_ALLOCFASTBITCACHE_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_ALLOCFASTBITCACHE_OFFSET))
#define OBC_FROM_ALLOCFASTBITCACHE_VAL(rayVal) ((void**)(((void*)(&(rayVal))) + _OBC_ALLOCFASTBITCACHE_OFFSET))

typedef struct OBC_AllocFastBitCache{

    OBC_Ray backed;

    OBC_ALLOCFASTBITCACHE_META metaCache;
    unsigned int metaCachePos;
    unsigned int maxHead;

    OBC_Ray meta;

#ifdef OBC_ALLOCFASTBITCACHE_STACK_SIZE
#if OBC_ALLOCFASTBITCACHE_STACK_SIZE < 1
#undef OBC_ALLOCFASTBITCACHE_STACK_SIZE
#endif // OBC_ALLOCFASTBITCACHE_STACK_SIZE
#endif // OBC_ALLOCFASTBITCACHE_STACK_SIZE


#ifdef OBC_ALLOCFASTBITCACHE_STACK_SIZE
    unsigned int freeStack[OBC_ALLOCFASTBITCACHE_STACK_SIZE];
#endif // OBC_ALLOCFASTBITCACHE_STACK_SIZE

    //OBC_ALLOCFASTBITCACHE_META noAllocCache;
    //unsigned int metaCachePos;

}OBC_AllocFastBitCache;

void OBC_AllocFastBitCacheSetHeadLink(OBC_AllocFastBitCache *allocator, OBC_ALLOCFASTBITCACHE_ADDR link);
OBC_ALLOCFASTBITCACHE_ADDR OBC_AllocFastBitCacheXetHeadLink(OBC_AllocFastBitCache *allocator);

void **OBC_newAllocFastBitCache(size_t unitSize);
OBC_ERROR_ENUM OBC_initAllocFastBitCache(OBC_AllocFastBitCache *allocator, size_t unitSize);
void OBC_freeAllocFastBitCache(void *allocator);
void OBC_freeAllocFastBitCacheData(void *allocator);

void **OBC_AllocFastBitCacheGetDataPointer(OBC_AllocFastBitCache *allocator);

unsigned int OBC_AllocFastBitCacheMalloc(void *allocator);
OBC_ERROR_ENUM OBC_AllocFastBitCacheFree(void *allocator, size_t pos);

unsigned int OBC_AllocFastBitCacheMallocRaw(OBC_AllocFastBitCache *allocator);


OBC_ERROR_ENUM OBC_AllocFastBitCacheExpand(void *allocator);
size_t OBC_AllocFastBitCacheGetFreeMeta(void *allocator);

OBC_ERROR_ENUM OBC_AllocFastBitCacheExpandRaw(OBC_AllocFastBitCache *allocator);

unsigned int OBC_AllocFastBitCacheBitPos(OBC_ALLOCFASTBITCACHE_META meta);
void OBC_AllocFastBitCacheFlushToMem(OBC_AllocFastBitCache *allocator);
unsigned int OBC_AllocFastBitCacheGetNextFree(OBC_AllocFastBitCache *allocator);

OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetLink(OBC_AllocFastBitCache *allocator, OBC_ALLOCFASTBITCACHE_ADDR linkPos);
OBC_ALLOCFASTBITCACHE_ADDR* OBC_AllocFastBitCacheGetHeadLink(OBC_AllocFastBitCache *allocator);


#endif // ALLOCFASTBITCACHE_H_INCLUDED
