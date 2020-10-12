#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "../obc.h"
#include "ray.h"

typedef struct OBC_Stack{

    OBC_Ray stack;

}OBC_Stack;

/*************************************
Initialization
*************************************/

void **OBC_newStack(size_t unitSize);
OBC_ERROR_ENUM OBC_initStack(OBC_Stack *stack, size_t unitSize);

/*************************************
Deallocation
*************************************/

void OBC_freeStack(void *arr);
void OBC_freeStackData(OBC_Stack *stack);

/*************************************
Data Accessors
*************************************/

void **OBC_StackGetDataPointer(OBC_Stack *stack);

/*************************************
Operations
*************************************/

OBC_Offset OBC_StackPushRaw(OBC_Stack *stack);
OBC_Offset OBC_StackPush(void *arr);

OBC_Offset OBC_StackPopRaw(OBC_Stack *stack);
OBC_Offset OBC_StackPop(void *arr);

OBC_ERROR_ENUM OBC_ListPushNewRaw(OBC_Stack *stack, void *item);
OBC_ERROR_ENUM OBC_ListPushNew(void *arr, void *item);


/*************************************
Iteration
*************************************/

typedef OBC_Offset OBC_StackIterator;
#define OBC_StackForEach(iter, arrPtr) for(iter = OBC_StackIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_StackIterNext(arrPtr, iter))
#define OBC_StackForEachRaw(iter, listPtr) for(iter = OBC_StackIterStartRaw(listPtr); iter != OBC_NULL_INDEX; iter = OBC_StackIterNextRaw(listPtr, iter))


/*************************************
Alternate Operations
*************************************/

#define OBC_StackPushNewFast(rawPtr, itemValue)\
{\
if(OBC_StackPush(rawPtr) != OBC_NULL_INDEX){\
   ((*(rawPtr))[OBC_TO_STACK_PTR(rawPtr)->stack.curUnitLength]) = itemValue;\
}\
}


/*************************************
Internal Utilities
*************************************/

#define _OBC_STACK_PTR_CAST(rawPtr) ((OBC_Stack *)(rawPtr))
#define _OBC_STACK_OFFSET ((size_t)(&((OBC_Stack *)NULL)->stack.rawData))
#define OBC_TO_STACK_PTR(rawPtr) (_OBC_STACK_PTR_CAST(((void*)(rawPtr)) - _OBC_STACK_OFFSET))

#define OBC_FROM_STACK_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_STACK_OFFSET))
#define OBC_FROM_STACK_VAL(stackVal) ((void**)(((void*)(&(stackVal))) + _OBC_STACK_OFFSET))



OBC_Offset OBC_StackIterStartRaw(OBC_Stack *stack);
OBC_Offset OBC_StackIterStart(void *arr);

OBC_Offset OBC_StackIterNextRaw(OBC_Stack *stack, OBC_Offset index);
OBC_Offset OBC_StackIterNext(void *arr, OBC_Offset index);

#endif // STACK_H_INCLUDED
