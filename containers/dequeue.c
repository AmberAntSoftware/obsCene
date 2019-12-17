#include "dequeue.h"

void **OBC_newDequeue(size_t unitSize){

    OBC_Dequeue *dequeue = calloc(1,sizeof(OBC_Dequeue));

    if(OBC_initDequeue(dequeue,unitSize) == NULL){
        free(dequeue);
        return NULL;
    }

    return (void **) &dequeue->backed.rawData;

}
void *OBC_initDequeue(OBC_Dequeue *dequeue, size_t unitSize){

    if(OBC_initRay(& dequeue->backed,0,unitSize) == NULL){
        return NULL;
    }

    return dequeue;
}
