#ifndef OBSCENEADT_H_INCLUDED
#define OBSCENEADT_H_INCLUDED

#include "obsceneinternals.h"

typedef struct OBCADT_Vec{

    size_t curLength;
    size_t maxLength;
    size_t unitSize;

    char *rawData;

}OBCADT_Vec;

#define __OBCADT_VecPTR(OBCADT_Vec_PTR) ((OBCADT_Vec *)(OBCADT_Vec_PTR))
#define _OBCADT_OFFSET_TO_DATA(OBCADT_Vec_PTR) ((__OBCADT_VecPTR(OBCADT_Vec_PTR)->rawData)
#define _OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR) __OBCADT_VecPTR(OBCADT_Vec_PTR-((size_t)(sizeof(OBCADT_Vec)-sizeof(char*))))

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
#define OBCADT_VecLength(OBCADT_Vec_PTR) ((__OBCADT_VecPTR(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->curLength)
#define OBCADT_VecMaxLength(OBCADT_Vec_PTR) ((__OBCADT_VecPTR(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->maxLength)
#define OBCADT_VecUnitSize(OBCADT_Vec_PTR)((__OBCADT_VecPTR(_OBCADT_REVERSE_VEC_OFFSET(OBCADT_Vec_PTR)))->unitSize)
#define OBCADT_VecGet(OBCADT_Vec_PTR, size_t_index) (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])
///#define OBCADT_VecSet(OBCADT_Vec_PTR, size_t_index) (OBCADT_Vec_PTR[size_t_index * OBCADT_VecUnitSize(OBCADT_Vec_PTR)])
void *OBC_newVec(size_t unitSize, size_t initialReserveSize);

#endif // OBSCENEADT_H_INCLUDED
