#include "obsceneadt.h"

void *OBC_newVec(size_t unitSize, size_t initialReserveSize){
    OBCADT_Vec *ptr = malloc(sizeof(OBCADT_Vec));
    ptr->maxLength = initialReserveSize;
    ptr->curLength = 0;
    ptr->unitSize = 0;
    ptr->rawData = malloc(sizeof(initialReserveSize*unitSize));
    ptr = ptr->rawData;
    return ptr;
}
