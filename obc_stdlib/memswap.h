#ifndef MEMSWAP_H_INCLUDED
#define MEMSWAP_H_INCLUDED

#include <string.h>

#include "../obc_natives.h"

///general good performance on noncontention, good for high contention resources .constprop
void memswap0(void* __restrict b, void* __restrict a, size_t sizeInBytes);
///general good performance on noncontention, good for high contention resources
void memswap1(void* __restrict b, void* __restrict a, size_t sizeInBytes);
///better performance on noncontenion not as good for high contention resources -- optimized out
void memswap2(void* __restrict b, void* __restrict a, size_t sizeInBytes);
///better performance on noncontenion not as good for high contention resources -- optimized out
void memswap3(void* __restrict b, void* __restrict a, size_t sizeInBytes);


#define memswap_inline(a,b, modified_sizeInBytes)\
{\
size_t sizeInBytes = modified_sizeInBytes;\
size_t i = 0;\
if(sizeInBytes >= sizeof(OBC_LONGEST)*4){\
    do{\
        sizeInBytes-=sizeof(OBC_LONGEST)*4;\
\
        OBC_LONGEST tmp0 = ((OBC_LONGEST *)a)[i];\
        ((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];\
        ((OBC_LONGEST *)b)[i] = tmp0;\
        tmp0 = ((OBC_LONGEST *)a)[i+1];\
        ((OBC_LONGEST *)a)[i+1] = ((OBC_LONGEST *)b)[i+1];\
        ((OBC_LONGEST *)b)[i+1] = tmp0;\
        tmp0 = ((OBC_LONGEST *)a)[i+2];\
        ((OBC_LONGEST *)a)[i+2] = ((OBC_LONGEST *)b)[i+2];\
        ((OBC_LONGEST *)b)[i+2] = tmp0;\
        tmp0 = ((OBC_LONGEST *)a)[i+3];\
        ((OBC_LONGEST *)a)[i+3] = ((OBC_LONGEST *)b)[i+3];\
        ((OBC_LONGEST *)b)[i+3] = tmp0;\
\
        i+=4;\
    }while(sizeInBytes > sizeof(OBC_LONGEST)*4);\
}\
\
if(sizeInBytes >= sizeof(OBC_LONGEST)){\
    do{\
        sizeInBytes-=sizeof(OBC_LONGEST);\
        OBC_LONGEST tmp0 = ((OBC_LONGEST *)a)[i];\
        ((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];\
        ((OBC_LONGEST *)b)[i] = tmp0;\
\
        i+=1;\
    }while(sizeInBytes > sizeof(OBC_LONGEST));\
}\
\
i*=sizeof(OBC_LONGEST);\
\
if(sizeInBytes){\
    do{\
        sizeInBytes--;\
        unsigned char tmp0 = ((unsigned char *)a)[i];\
        ((unsigned char *)a)[i] = ((unsigned char *)b)[i];\
        ((unsigned char *)b)[i] = tmp0;\
        i++;\
    }while(sizeInBytes);\
}\
}


#endif // MEMSWAP_H_INCLUDED
