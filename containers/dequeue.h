#ifndef DEQUEUE_H_INCLUDED
#define DEQUEUE_H_INCLUDED

#include "../obc.h"
#include "queue.h"

#define _OBC_DEQUEUE_PTR_CAST(rawPtr) ((OBC_Dequeue *)(rawPtr))
#define _OBC_DEQUEUE_OFFSET ((size_t)(&((OBC_Dequeue *)NULL)->queue.backed.rawData))
#define OBC_TO_DEQUEUE_PTR(rawPtr) (_OBC_DEQUEUE_PTR_CAST(((void*)(rawPtr)) - _OBC_DEQUEUE_OFFSET))

#define OBC_FROM_DEQUEUE_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_DEQUEUE_OFFSET))
#define OBC_FROM_DEQUEUE_VAL(dequeueVal) ((void**)(((void*)(&(dequeueVal))) + _OBC_DEQUEUE_OFFSET))

typedef struct OBC_Dequeue{

    OBC_Queue queue;

}OBC_Dequeue;

/*************************************
Initialization
*************************************/

void **OBC_newDequeue(size_t unitSize);
OBC_ERROR_ENUM OBC_initDequeue(OBC_Dequeue *dequeue, size_t unitSize);

/*************************************
Deallocation
*************************************/

void OBC_freeDequeue(void *arr);
void OBC_freeDequeueData(OBC_Dequeue *dequeue);

/*************************************
Data Accessors
*************************************/

void **OBC_DequeueGetDataPointer(OBC_Dequeue *dequeue);

/*************************************
Utilities
*************************************/

OBC_Offset OBC_DequeuePopFrontRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePopBackRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePopFront(void *arr);
OBC_Offset OBC_DequeuePopBack(void *arr);

OBC_Offset OBC_DequeuePushFrontRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePushBackRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePushFront(void *arr);
OBC_Offset OBC_DequeuePushBack(void *arr);

OBC_ERROR_ENUM OBC_DequeueAddFrontRaw(OBC_Dequeue *dequeue, void* item);
OBC_ERROR_ENUM OBC_DequeueAddBackRaw(OBC_Dequeue *dequeue, void* item);
OBC_ERROR_ENUM OBC_DequeueAddFront(void *arr, void* item);
OBC_ERROR_ENUM OBC_DequeueAddBack(void *arr, void* item);

OBC_ERROR_ENUM OBC_DequeueAdd(void *arr, OBC_Offset pos, void* item);
OBC_ERROR_ENUM OBC_DequeueAddRaw(OBC_Dequeue *dequeue, OBC_Offset pos, void* item);



typedef OBC_Offset OBC_DequeueIterator;

#define OBC_DequeueForEach(arrPtr, iter) for(iter = OBC_DequeueIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_DequeueIterNext(arrPtr, iter))
#define OBC_DequeueForEachRaw(dequeuePtr, iter) for(iter = OBC_DequeueIterStartRaw(dequeuePtr); iter != OBC_NULL_INDEX; iter = OBC_DequeueIterNextRaw(dequeuePtr, iter))

OBC_Offset OBC_DequeueIterStartRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeueIterStart(void *arr);

OBC_Offset OBC_DequeueIterNextRaw(OBC_Dequeue *dequeue, OBC_Offset iter);
OBC_Offset OBC_DequeueIterNext(void *arr, OBC_Offset iter);

#endif // DEQUEUE_H_INCLUDED
