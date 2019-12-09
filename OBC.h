#ifndef OBC_H_INCLUDED
#define OBC_H_INCLUDED

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void *OBC_memset (void *ptr, int c, size_t len);



/*
typedef struct OBC_BigTimer{

}OBC_BigTimer;

typedef size_t OBC_BigMilliseconds;

size_t OBC_BigClock(clock_t start, clock_t end);
*/
unsigned int OBC_clockMillis(clock_t start, clock_t end);
unsigned int OBC_clockSeconds(clock_t start, clock_t end);

/*
#define INTERFACE(returnValue, functionName, argsListWithParenthesis)\
    returnValue (* functionName) argsListWithParenthesis

#define IMPLEMENT_INTERFACE(interface,typename)

typedef union I_Allocator{

    INTERFACE(void**,New,(size_t unitSize));
    INTERFACE(void*,init,(void *allocator, size_t unitSize));
    INTERFACE(void,free,(void *allocator));
    INTERFACE(void,freeData,(void *allocator));
    INTERFACE(OBC_ERROR_ENUM,markAllocated,(void *allocator, size_t pos));
    INTERFACE(OBC_ERROR_ENUM,expand,(void *allocator));
    INTERFACE(size_t,getFreeLocation,(void *allocator));
    INTERFACE(size_t,malloc,(void *allocator));
    INTERFACE(OBC_ERROR_ENUM,allocatorFree,(void *allocator, size_t pos));
    INTERFACE(OBC_ERROR_ENUM,markFreed,(void *allocator, size_t pos));

}I_Allocator;
*/

#endif // OBC_H_INCLUDED
