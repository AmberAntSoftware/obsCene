#include "obsceneadt.h"

void *OBC_newVec(size_t unitSize, size_t initialReserveSize){
    OBCADT_Vec *ptr = malloc(sizeof(OBCADT_Vec));
    if(ptr == NULL){
        return NULL;
    }
    ptr->maxLength = initialReserveSize;
    ptr->curLength = 0;
    ptr->unitSize = 0;
    ptr->rawData = malloc(sizeof(initialReserveSize*unitSize));
    if(ptr->rawData == NULL){
        free(ptr);
        return NULL;
    }
    return (void *)ptr->rawData;
}

//size_t OBCADT_VecAdd(void *rawPtr);
void OBCADT_VecFree(void *rawPtr){
    free((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(rawPtr))));
}
size_t OBCADT_VecLength(void *rawPtr){
    return ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(rawPtr)))->curLength);
}
size_t OBCADT_VecMaxLength(void *rawPtr){
    return ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(rawPtr)))->maxLength);
}
size_t OBCADT_VecUnitSize(void *rawPtr){
    return ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(rawPtr)))->unitSize);
}
///returns pointer to data, does not deref -- "prevent internal segfault for bounds issues"
void *OBCADT_VecGet(void *rawPtr, size_t index){
    return (void *)(((char *)rawPtr)+(index*((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(rawPtr)))->unitSize)));
}
void OBCADT_VecSet(void *rawPtr, size_t index);// (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])

void OBCADT_VecApplyAll(void *rawPtr, void (*func)(void *dataPtr)){
    size_t unitSize = OBCADT_VecUnitSize(rawPtr);
    size_t length = OBCADT_VecLength(rawPtr);
    size_t i;
    char *ptr = rawPtr;
    for(i = 0; i < length; i++){
        func(ptr);
        rawPtr+=unitSize;
    }
}
