#ifndef OBC_H_INCLUDED
#define OBC_H_INCLUDED

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

#ifndef OBC_NULL_INDEX
    #define OBC_NULL_INDEX (~((size_t)0))
#endif // NULL

#ifndef OBC_MEMORY_FAILURE
    #define OBC_MEMORY_FAILURE 1
#endif // NULL

#if DEBUG
    #define debug(...) printf(__VA_ARGS__)
#else
    #define debug(...) do{}while(0);
#endif // 1

///TODO may need to supply PDP endianess later
#if 16777216 & 0x00000001 != 0
    #define ENDIAN_BIG 1
#else
    #define ENDIAN_LITTLE 1
#endif // 1

typedef enum OBC_ERROR_ENUM{
    OBC_ERROR_NO_OP = 0,
    OBC_ERROR_SUCCESS,
    OBC_ERROR_FAILURE
}OBC_ERROR_ENUM;

typedef struct OBC_ErrorPointer{
    OBC_ERROR_ENUM errorValue;
    void *realValue;
}OBC_ErrorPointer;


#endif // OBC_H_INCLUDED
