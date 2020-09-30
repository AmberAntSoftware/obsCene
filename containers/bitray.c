#include "bitray.h"


OBC_BitRay* OBC_newBitRay(){

    OBC_BitRay *bray = calloc(1,sizeof(OBC_BitRay));

    if(OBC_initBitRay(bray) == OBC_ERROR_FAILURE){
        free(bray);
        return NULL;
    }

    return bray;

}
OBC_ERROR_ENUM OBC_initBitRay(OBC_BitRay *bray){

    if(OBC_initRay(& bray->bits,0,sizeof(OBC_LONGEST)) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}

void OBC_freeBitRay(OBC_BitRay *bray){

    OBC_freeBitRayData(bray);
    free(bray);

}
void OBC_freeBitRayData(OBC_BitRay *bray){

    OBC_freeRayData(& bray->bits);

}

/*
void **OBC_BitRayFirst0Bit(OBC_BitRay *bray){

}*/

OBC_ValueBit OBC_BitRayDoSet(OBC_BitRay *bray, size_t pos, unsigned char bit){
    if(OBC_BitRayDoExpand(bray, pos) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*CHAR_BIT);
    const char mask = bit << bits;
    bray->bits.rawData[byte] &= ~mask;
    bray->bits.rawData[byte] ^= mask;

    return OBC_ERROR_SUCCESS;
}
OBC_ValueBit OBC_BitRayDoGet(OBC_BitRay *bray, size_t pos){
    if(OBC_BitRayDoExpand(bray, pos) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*CHAR_BIT);
    const char mask = 1 << bits;

    return (bray->bits.rawData[byte]&mask) >> bits;
}

unsigned char OBC_BitRayGet(OBC_BitRay *bray, size_t pos){
    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*CHAR_BIT);
    const char mask = 1 << bits;

    return (bray->bits.rawData[byte]&mask) >> bits;
}
void OBC_BitRaySet(OBC_BitRay *bray, size_t pos, unsigned char bit){
    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*CHAR_BIT);
    const char mask = bit << bits;
    bray->bits.rawData[byte] &= ~mask;
    bray->bits.rawData[byte] ^= mask;
}

OBC_Offset OBC_BitRayFirstLeft0Bit(OBC_BitRay *bray, size_t pos){

    const OBC_LONGEST *const bits = (OBC_LONGEST *)bray->bits.rawData;
    const OBC_LONGEST const FULL = BITRAY_FULL_VALUE;
    size_t i,j;

//_mm256_testz_si256()
//_mm_test
//_mm_testc_si128
//_mm512_mask_test
//_mm512_set1_epi64()
//_mm256_set1_epi64x()
//_mm_set1_epi64x()
//_mm512_test_epi64_mask()
//_mm512_cmpeq_epi64_mask()

//_mm_testc_si128

    for(i=0; i < bray->bits.maxUnitLength; i++){
        //if(bits[i] != NON_EMPTY){
        if(BITRAY_FULL_CHECK(bits[i],FULL)){
            break;
        }
    }
    if(i == bray->bits.maxUnitLength){
        return OBC_NULL_INDEX;
    }

    i*=sizeof(OBC_LONGEST);
    bray->bits.rawData;
    for(j=0; j < sizeof(OBC_LONGEST); j++){
        if(bray->bits.rawData[i+j] != (~((char)0)) ){
            break;
        }
    }

    i+=j;
    for(j=0; j < CHAR_BIT; j++){
        if(((bray->bits.rawData[i]>>j) & 1) == 0){
            return i+j;
        }
    }

    return OBC_NULL_INDEX;
}

/*
OBC_Offset OBC_BitRayFirstLeft1Bit(OBC_BitRay *bray, size_t pos){

}

OBC_Offset OBC_BitRayFirstRight1Bit(OBC_BitRay *bray, size_t pos){

}
OBC_Offset OBC_BitRayFirstRight0Bit(OBC_BitRay *bray, size_t pos){

}
*/

OBC_ERROR_ENUM OBC_BitRayDoExpand(OBC_BitRay *bray, size_t bitPosNeeded){
    if(bray->bits.maxUnitLength * OBC_BITRAY_META_BITS <= bitPosNeeded){
        return OBC_RayExpand(OBC_FROM_RAY_VAL(bray->bits));
    }
    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_BitRayExpand(OBC_BitRay *bray){
    if(OBC_RayExpand(OBC_FROM_RAY_VAL(bray->bits))== OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    return OBC_ERROR_SUCCESS;
}


