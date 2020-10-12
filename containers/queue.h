#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "../obc.h"
#include "ray.h"

typedef struct OBC_Queue{

    OBC_Ray backed;

    OBC_Offset head;
    OBC_Offset count;

}OBC_Queue;

/*************************************
Initialization
*************************************/

void **OBC_newQueue(size_t unitSize);

OBC_ERROR_ENUM OBC_initQueue(OBC_Queue *queue, size_t unitSize);

/*************************************
Deallocation
*************************************/

void OBC_freeQueue(void *arr);
void OBC_freeQueueData(OBC_Queue *queue);

/*************************************
Data Accessors
*************************************/

void **OBC_QueueGetDataPointer(OBC_Queue *queue);

/*************************************
Operations
*************************************/

OBC_Offset OBC_QueuePop(void *arr);
OBC_Offset OBC_QueuePopRaw(OBC_Queue *queue);

OBC_Offset OBC_QueuePush(void *arr);
OBC_Offset OBC_QueuePushRaw(OBC_Queue *queue);

OBC_ERROR_ENUM OBC_QueueAdd(void *arr, void *item);
OBC_ERROR_ENUM OBC_QueueAddRaw(OBC_Queue *queue, void *item);

/*************************************
Iteration
*************************************/

typedef OBC_Offset OBC_QueueIterator;

#define OBC_QueueForEach(arrPtr, iter) for(iter = OBC_QueueIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_QueueIterNext(arrPtr, iter))
#define OBC_QueueForEachRaw(queuePtr, iter) for(iter = OBC_QueueIterStartRaw(queuePtr); iter != OBC_NULL_INDEX; iter = OBC_QueueIterNextRaw(queuePtr, iter))

OBC_Offset OBC_QueueIterStartRaw(OBC_Queue *queue);
OBC_Offset OBC_QueueIterStart(void *arr);

OBC_Offset OBC_QueueIterNextRaw(const OBC_Queue *const queue, const OBC_Offset index);
OBC_Offset OBC_QueueIterNext(const void *const arr, const OBC_Offset index);

/*************************************
Internal Utilities
*************************************/

#define _OBC_QUEUE_PTR_CAST(rawPtr) ((OBC_Queue *)(rawPtr))
#define _OBC_QUEUE_OFFSET ((size_t)(&((OBC_Queue *)NULL)->backed.rawData))
#define OBC_TO_QUEUE_PTR(rawPtr) (_OBC_QUEUE_PTR_CAST(((void*)(rawPtr)) - _OBC_QUEUE_OFFSET))

#define OBC_FROM_QUEUE_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_QUEUE_OFFSET))
#define OBC_FROM_QUEUE_VAL(queueVal) ((void**)(((void*)(&(queueVal))) + _OBC_QUEUE_OFFSET))



OBC_ERROR_ENUM OBC_QueueExpandRaw(OBC_Queue *queue);
OBC_ERROR_ENUM OBC_QueueExpand(void *arr);

#endif // QUEUE_H_INCLUDED
