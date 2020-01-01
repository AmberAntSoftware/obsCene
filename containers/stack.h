#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "OBC.h"
#include "ray.h"

#define _OBC_STACK_PTR_CAST(rawPtr) ((OBC_Stack *)(rawPtr))
#define _OBC_STACK_OFFSET ((size_t)(&((OBC_Stack *)NULL)->stack.rawData))
#define OBC_TO_STACK_PTR(rawPtr) (_OBC_STACK_PTR_CAST(((void*)(rawPtr)) - _OBC_STACK_OFFSET))

#define OBC_FROM_STACK_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_STACK_OFFSET))
#define OBC_FROM_STACK_VAL(stackVal) ((void**)(((void*)(&(stackVal))) + _OBC_STACK_OFFSET))

typedef struct OBC_Stack{

    OBC_Ray stack;

}OBC_Stack;

void **OBC_newStack(size_t elementSize);
void *OBC_initStack(OBC_Stack *stack,size_t unitSize);
void **OBC_StackGetAccessPointer(OBC_Stack *stack);

void OBC_freeStack(void *raw);
void OBC_freeStackData(OBC_Stack *stack);

OBC_Offset OBC_StackPushRaw(OBC_Stack *stack);
OBC_Offset OBC_StackPush(void *raw);

OBC_Offset OBC_StackPopRaw(OBC_Stack *stack);
OBC_Offset OBC_StackPop(void *raw);

OBC_ERROR_ENUM OBC_ListPushNewRaw(OBC_Stack *stack, void *item);
OBC_ERROR_ENUM OBC_ListPushNew(void *raw, void *item);



typedef OBC_Offset OBC_StackIterator;

#define OBC_StackForEach(iter, arrPtr) for(iter = OBC_StackIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_StackIterNext(arrPtr, iter))
#define OBC_StackForEachRaw(iter, listPtr) for(iter = OBC_StackIterStartRaw(listPtr); iter != OBC_NULL_INDEX; iter = OBC_StackIterNextRaw(listPtr, iter))

OBC_Offset OBC_StackIterStartRaw(OBC_Stack *stack);
OBC_Offset OBC_StackIterStart(void *raw);

OBC_Offset OBC_StackIterNextRaw(OBC_Stack *stack, OBC_Offset iter);
OBC_Offset OBC_StackIterNext(void *raw, OBC_Offset iter);

#define OBC_StackPushNewFast(rawPtr, itemValue)\
{\
if(OBC_StackPush(rawPtr) != OBC_NULL_INDEX){\
   ((*(rawPtr))[OBC_TO_STACK_PTR(rawPtr)->stack.curUnitLength]) = itemValue;\
}\
}

#endif // STACK_H_INCLUDED
