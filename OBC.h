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

//https://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
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

typedef size_t OBC_Offset;
typedef OBC_Offset OBC_Iterator;

void *OBC_memset (void *ptr, int c, size_t len);

/*
typedef struct OBC_BigTimer{

}OBC_BigTimer;

typedef size_t OBC_BigMilliseconds;

size_t OBC_BigClock(clock_t start, clock_t end);
*/
typedef clock_t OBC_Timer;
#define OBC_newTimer() clock()
unsigned int OBC_timerMillis(const OBC_Timer start, const OBC_Timer end);
unsigned int OBC_timerSeconds(const OBC_Timer start, const OBC_Timer end);

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

#define OBC_X_MEMCPY(voidCharDataPtr,unitSize, unit, itemPtr)\
if(unitSize > 512){\
    memcpy(voidCharDataPtr+(unitSize*unit),item,unitSize);\
}else{\
size_t where = 0;\
\
    void *dataX = voidCharDataPtr+(unitSize*unit);\
\
    if(sizeof(unsigned int)*8 < unitSize){\
        do{\
            *((unsigned int *)(dataX+where))\
                = *((unsigned int *)(itemPtr+where));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*2))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*2));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*3))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*3));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*4))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*4));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*5))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*5));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*6))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*6));\
            *((unsigned int *)(dataX+where+sizeof(unsigned int)*7))\
                = *((unsigned int *)(itemPtr+where+sizeof(unsigned int)*7));\
\
            where+=sizeof(unsigned int)*8;\
        }while(where+sizeof(unsigned int)*8 < unitSize);\
    }\
\
    if(where+sizeof(unsigned int) < unitSize){\
        do{\
            *((unsigned int *)(dataX+where))\
                = *((unsigned int *)(itemPtr+where));\
            where+=sizeof(unsigned int);\
        }while(where < unitSize);\
    }\
\
    if(where < unitSize){\
        do{\
            *((unsigned char *)(dataX+where))\
                = *((unsigned char *)(itemPtr+where));\
            where+=sizeof(unsigned char);\
        }while(where < unitSize);\
    }\
}

#endif // OBC_H_INCLUDED
