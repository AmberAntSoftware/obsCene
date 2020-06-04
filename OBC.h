#ifndef OBC_H_INCLUDED
#define OBC_H_INCLUDED

#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

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

typedef clock_t OBC_Timer;
#define OBC_newTimer() clock()
unsigned int OBC_timerMillis(const OBC_Timer start, const OBC_Timer end);
unsigned int OBC_timerSeconds(const OBC_Timer start, const OBC_Timer end);


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
