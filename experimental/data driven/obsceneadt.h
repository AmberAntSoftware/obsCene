#ifndef OBSCENEADT_H_INCLUDED
#define OBSCENEADT_H_INCLUDED

#include "obsceneinternals.h"

//#define OBCADT__VEC_PREPROCESSOR

typedef struct OBCADT_Vec{

    size_t curLength;
    size_t maxLength;
    size_t unitSize;

    char *rawData;

}OBCADT_Vec;



#define _OBCADT_VecPTR_CAST(OBCADT_Vec_PTR) ((OBCADT_Vec *)(OBCADT_Vec_PTR))
#define _OBCADT_OFFSET_TO_DATA(OBCADT_Vec_PTR) ((_OBCADT_VecPTR_CAST(OBCADT_Vec_PTR)->rawData)
#define _OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR) _OBCADT_VecPTR_CAST(OBCADT_Vec_PTR-((size_t)(sizeof(OBCADT_Vec)-sizeof(char*))))

#ifdef OBCADT__VEC_PREPROCESSOR
#define OBCADT_VecUnitLen(OBCADT_Vec_PTR) ((OBCADT_Vec *)(OBCADT_Vec_PTR))
#define OBCADT_VecAdd(OBCADT_Vec_PTR) do{ \
                                        if(OBCADT_VecLength(OBCADT_Vec_PTR) >= OBCADT_VecMaxLength(OBCADT_Vec_PTR)){ \
                                            OBCADT_VecMaxLength(OBCADT_Vec_PTR)*=2; \
                                        realloc(OBCADT_Vec_PTR, OBCADT_VecMaxLength(OBCADT_Vec_PTR)); \
                                        } \
                                        OBCADT_VecLength(OBCADT_Vec_PTR)++;\
                                        \
                                        \
                                    }while(0);
#define OBCADT_VecFree(OBCADT_Vec_PTR) do { free(OBCADT_Vec_PTR); free(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)); }while(0);
#define OBCADT_VecLength(OBCADT_Vec_PTR) ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->curLength)
#define OBCADT_VecMaxLength(OBCADT_Vec_PTR) ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->maxLength)
#define OBCADT_VecUnitSize(OBCADT_Vec_PTR)((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->unitSize)
#define OBCADT_VecGet(OBCADT_Vec_PTR, size_t_index) (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])
///#define OBCADT_VecSet(OBCADT_Vec_PTR, size_t_index) (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])

#define OBCADT_VecApplyAll(OBCADT_Vec_PTR, funcPtrOnePtrParam) \
                                    do{\
                                    size_t OBC__VEC_COUNT = 0;\
                                    for(;OBC__VEC_COUNT < OBCADT_VecLength(OBCADT_Vec_PTR); OBC__VEC_COUNT+=OBCADT_VecUnitSize(OBCADT_Vec_PTR)){\
                                        funcPtrOnePtrParam(((char *)OBCADT_Vec_PTR)+OBC__VEC_COUNT);\
                                    }\
                                    }while(0);

#else


void *OBC_newVec(size_t unitSize, size_t initialReserveSize);

size_t OBCADT_VecUnitLenth(void *rawPtr);
size_t OBCADT_VecAdd(void *rawPtr);
void OBCADT_VecFree(void *rawPtr);// do { free(OBCADT_Vec_PTR); free(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)); }while(0);
size_t OBCADT_VecLength(void *rawPtr);// ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->curLength)
size_t OBCADT_VecMaxLength(void *rawPtr);// ((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->maxLength)
size_t OBCADT_VecUnitSize(void *rawPtr);//((_OBCADT_VecPTR_CAST(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->unitSize)
///returns pointer to data, does not deref -- "prevent internal segfault for bounds issues"
void *OBCADT_VecGet(void *rawPtr, size_t index);// (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])
void OBCADT_VecSet(void *rawPtr, size_t index);// (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])

void OBCADT_VecApplyAll(void *rawPtr, void (*func)(void *dataPtr));

#endif // OBCADT__VEC_PREPROCESSOR

#endif // OBSCENEADT_H_INCLUDED
