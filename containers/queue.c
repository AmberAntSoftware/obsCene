#include "queue.h"

void **OBC_newQueue(size_t unitSize){

    OBC_Queue *queue = calloc(1,sizeof(OBC_Queue));

    if(OBC_initQueue(queue,unitSize) == OBC_ERROR_FAILURE){
        free(queue);
        return NULL;
    }

    return OBC_QueueGetDataPointer(queue);

}
OBC_ERROR_ENUM OBC_initQueue(OBC_Queue *queue, size_t unitSize){

    if(OBC_initRayComplex(& queue->backed,0,unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    queue->head = 0;
    queue->count = 0;

    return OBC_ERROR_SUCCESS;
}
void **OBC_QueueGetDataPointer(OBC_Queue *queue){
    return (void **) OBC_FROM_RAY_VAL(queue->backed);
}

void OBC_freeQueue(void *arr){
    OBC_Queue *queue = OBC_TO_QUEUE_PTR(arr);
    OBC_freeQueueData(queue);
    free(queue);
}
void OBC_freeQueueData(OBC_Queue *queue){
    OBC_freeRayData(& queue->backed);
}

OBC_Offset OBC_QueuePopRaw(OBC_Queue *queue){
    if(queue->count < 1){
        return queue->head;
    }
    queue->count--;
    queue->head++;
    if(queue->head >= queue->backed.maxUnitLength){
        ///TODO determine if max -1
        queue->head=0;
    }
    return queue->head;
}

OBC_Offset OBC_QueuePop(void *arr){
    return OBC_QueuePopRaw(OBC_TO_QUEUE_PTR(arr));
}

OBC_Offset OBC_QueuePushRaw(OBC_Queue *queue){
    queue->count++;
    if(queue->count >= queue->backed.maxUnitLength){
        if(OBC_QueueExpandRaw(queue) == OBC_ERROR_FAILURE){
            queue->count--;
            return OBC_NULL_INDEX;
        }
    }
    size_t place = queue->head + queue->count -1;
    if(place >= queue->backed.maxUnitLength){
        place-=queue->backed.maxUnitLength;
    }
    return place;
}
OBC_Offset OBC_QueuePush(void *arr){
    return OBC_QueuePushRaw(OBC_TO_QUEUE_PTR(arr));
}

OBC_ERROR_ENUM OBC_QueueAddRaw(OBC_Queue *queue, void *item){
    const size_t pos = OBC_QueuePushRaw(queue);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    OBC_X_MEMCPY(queue->backed.rawData,queue->backed.unitSize,pos,item);

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_QueueAdd(void *raw, void *item){
    return OBC_QueueAddRaw(OBC_TO_QUEUE_PTR(raw),item);
}







OBC_ERROR_ENUM OBC_QueueExpandRaw(OBC_Queue *queue){
    const size_t end = queue->backed.maxUnitLength;

    if(OBC_RayExpand(OBC_FROM_RAY_VAL(queue->backed)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    //regular full, no moving data necessary

    size_t place = queue->head+queue->count;

    if(queue->head == 0 || place < end){
        return OBC_ERROR_SUCCESS;
    }

    place-=end;

    if(place < end - queue->head){
        memcpy(queue->backed.rawData+end*queue->backed.unitSize,
               queue->backed.rawData, place*queue->backed.unitSize);
        return OBC_ERROR_SUCCESS;
    }

    const size_t offset = queue->head*queue->backed.unitSize;
    const size_t size = (end - queue->head)*queue->backed.unitSize;
    memcpy(queue->backed.rawData+offset
           ,queue->backed.rawData+(queue->backed.maxUnitLength*queue->backed.unitSize) - size
           ,size);

    queue->head = queue->backed.maxUnitLength - queue->head;

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_QueueExpand(void *arr){
    return OBC_QueueExpandRaw(OBC_TO_QUEUE_PTR(arr));
}

OBC_Offset OBC_QueueIterStartRaw(OBC_Queue *queue){
    return queue->head;
}
OBC_Offset OBC_QueueIterStart(void *arr){
    return OBC_QueueIterStartRaw(OBC_TO_QUEUE_PTR(arr));
}

OBC_Offset OBC_QueueIterNextRaw(const OBC_Queue *const queue, const OBC_Offset iter){

    if(queue->count == queue->backed.maxUnitLength && iter+1 == queue->head){
        return OBC_NULL_INDEX;
    }

    ///NOTE: if statements can be replaced if PoT sizes are enforced in the
    ///allocation backend with & of the PoT size
    size_t len;
    if(iter >= queue->head){
        len = iter - queue->head;
    }else{
        len = (queue->backed.maxUnitLength - queue->head) + iter;
    }

    if(len+1 >= queue->count){
        return OBC_NULL_INDEX;
    }

    if(iter >= queue->backed.maxUnitLength){
        return 0;
    }
    return iter+1;
}
OBC_Offset OBC_QueueIterNext(const void *const arr, const OBC_Offset iter){
    return OBC_QueueIterNextRaw(OBC_TO_QUEUE_PTR(arr), iter);
}
