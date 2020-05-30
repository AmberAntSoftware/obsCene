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
    #define OBC_NULL_INDEX (~((OBC_Offset)0))
#endif // NULL

#ifdef DEBUG
    #define debug(...) printf(__VA_ARGS__)
#else
    #define debug(...) do{}while(0);
#endif // 1


//https://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
#define OBC_ENDIAN_ORDER  ('ABCD')
#define OBC_LITTLE_ENDIAN 0x41424344UL
#define OBC_BIG_ENDIAN    0x44434241UL
#if OBC_ENDIAN_ORDER == OBC_LITTLE_ENDIAN
    #undef OBC_ENDIAN_ORDER
    #undef OBC_LITTLE_ENDIAN
    #undef OBC_BIG_ENDIAN
    #define ENDIAN_LITTLE 1
#elif OBC_ENDIAN_ORDER == OBC_BIG_ENDIAN
    #undef OBC_ENDIAN_ORDER
    #undef OBC_LITTLE_ENDIAN
    #undef OBC_BIG_ENDIAN
    #define ENDIAN_BIG 1
#endif

#define OBC_X_ITERATOR(arrPtr, iter, startFunction, incrmentFunction) for(iter = startFunction(arrPtr); iter != OBC_NULL_INDEX; iter = incrmentFunction(arrPtr, iter))

typedef signed int OBC_SpaceshipValue;
typedef unsigned int OBC_Offset;
typedef unsigned int OBC_Iterator;

typedef enum OBC_ERROR_ENUM{
    OBC_ERROR_NO_OP = 0,
    OBC_ERROR_SUCCESS,
    OBC_ERROR_FAILURE
}OBC_ERROR_ENUM;

typedef struct OBC_ErrorPointer{
    OBC_ERROR_ENUM errorValue;
    void *realValue;
}OBC_ErrorPointer;

typedef struct OBC_LargeOffset{
    OBC_Offset row;
    OBC_Offset col;
}OBC_LargeOffset;

typedef void** OBC_Data;

#define obc(shortOffset) 0][shortOffset
#define OBC(largeOffset) 0][largeOffset.row][largeOffset.col

#define OBC_(shortOffset) 0][shortOffset
#define OBC__(largeOffset) 0][largeOffset.row][largeOffset.col


void *OBC_memset (void *ptr, int c, size_t len);

#define OBC_GetPointer(arrPtr, index) ((*(arrPtr))+index)
#define OBC_GetValue(arrPtr, index) ((*(arrPtr))[index])

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

/**
interface Allocator{

void **new(size_t unitSize);
void *init (interface *allocator, size_t unitSize);
void freeAllocator(interface *allocator);
void freeAllocatorData(interface *allocator);
size_t malloc(interface *allocator);
OBC_ERROR_ENUM free(interface *allocator, size_t data);

};

implement Allocator{

    ///required named functions, must match prototypes
    Allocator:new = OBC_newAllocFast;
    Allocator:init = OBC_initAllocFast;
    Allocator:freeAllocator = OBC_freeAllocFast;
    Allocator:freeAllocatorData = OBC_freeAllocFastData;
    Allocator:malloc = OBC_AllocFastMalloc;
    Allocator:free = OBC_AllocFastFree;

    ///other functions accessible from type
    markAllocated = OBC_AllocFastMarkAllocated;
    markFreed = OBC_AllocFastMarkFreed;

}FreeListAllocator;

void foobar(){

    FreeListAllocator allocator;

    allocator:init(& allocator);
    size_t item = allocator:malloc();

    //32bit print
    printf("Allocated At Position: %u\n", item);

    allocator:freeAllocatorData(& allocator);

}
**/

#endif // OBC_H_INCLUDED
