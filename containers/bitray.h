#ifndef BITRAY_H_INCLUDED
#define BITRAY_H_INCLUDED

#include "../obc.h"
#include "ray.h"
#include "../obc_natives.h"


#define OBC_BITRAY_META_BITS (sizeof(OBC_LONGEST)*CHAR_BIT)

typedef struct OBC_BitRay{

    OBC_Ray bits;

}OBC_BitRay;

typedef OBC_ERROR_ENUM OBC_ValueBit;

OBC_BitRay* OBC_newBitRay();
OBC_ERROR_ENUM OBC_initBitRay(OBC_BitRay *bitray);

void OBC_freeBitRay(OBC_BitRay *bray);
void OBC_freeBitRayData(OBC_BitRay *bitray);

OBC_Offset OBC_BitRayFirstLeft0Bit(OBC_BitRay *bray, size_t pos);

OBC_ValueBit OBC_BitRayDoSet(OBC_BitRay *bray, size_t pos, unsigned char bit);
OBC_ValueBit OBC_BitRayDoGet(OBC_BitRay *bray, size_t pos);

unsigned char OBC_BitRayGet(OBC_BitRay *bray, size_t pos);
void OBC_BitRaySet(OBC_BitRay *bray, size_t pos, unsigned char bit);


OBC_ERROR_ENUM OBC_BitRayDoExpand(OBC_BitRay *bray, size_t bitPosNeeded);
OBC_ERROR_ENUM OBC_BitRayExpand(OBC_BitRay *bray);



//0xFFFFFFFFFFFFFFFFLLU
#if AVX_BIT_SIZE == 512

#define BITRAY_FULL_VALUE _mm512_set1_epi64(0xFFFFFFFFFFFFFFFFLLU)
#define BITRAY_FULL_CHECK(valueToCheck, maxValue) \
(( ~_mm512_cmpeq_epi64_mask((maxValue),(valueToCheck)) ) == 0)


#else
#if AVX_BIT_SIZE == 256

#define BITRAY_FULL_VALUE _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFFLLU)
#define BITRAY_FULL_CHECK(valueToCheck, maxValue) \
( _mm256_testc_si256((maxValue),(valueToCheck)) )

#else
#if AVX_BIT_SIZE == 128

#define BITRAY_FULL_VALUE _mm_set1_epi64x(0xFFFFFFFFFFFFFFFFLLU)
#define BITRAY_FULL_CHECK(valueToCheck, maxValue) \
( _mm_testc_si128((maxValue),(valueToCheck)) )

#else

#define BITRAY_FULL_VALUE 0xFFFFFFFFFFFFFFFFLLU
#define BITRAY_FULL_CHECK(valueToCheck, maxValue) \
( (( (valueToCheck) != (maxValue) )) )

#endif // AVX_BIT_SIZE
#endif // AVX_BIT_SIZE
#endif // AVX_BIT_SIZE


#endif // BITRAY_H_INCLUDED
