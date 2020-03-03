#include "bitray.h"


void **OBC_newBitRay(){

    OBC_BitRay *bray = calloc(1,sizeof(OBC_BitRay));

    if(OBC_initBitRay(bray) == NULL){
        free(bray);
        return NULL;
    }

    return (void **)OBC_FROM_BITRAY_VAL(bray->bits);

}
void *OBC_initBitRay(OBC_BitRay *bray){

    if(OBC_initRay(& bray->bits,0,sizeof(X_X_LONGEST)) == NULL){
        return NULL;
    }

    return bray;
}
void **OBC_BitRayGetAccessPointer(OBC_BitRay *bray){
    return (void **)OBC_FROM_RAY_VAL(bray->bits);
}

void OBC_freeBitRay(void *raw){

    OBC_BitRay *bray = OBC_TO_BITRAY_PTR(raw);
    OBC_freeBitRayData(bray);
    free(bray);

}
void OBC_freeBitRayData(OBC_BitRay *bray){

    OBC_freeRayData(&bray->bits);

}

/*
void **OBC_BitRayFirst0Bit(OBC_BitRay *bray){

}*/

OBC_ValueBit OBC_BitRayDoSet(OBC_BitRay *bray, size_t pos, unsigned char bit){
    if(OBC_BitRayDoExpand(bray, pos) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*char_BIT);
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
    const size_t bits = pos-(byte*char_BIT);
    const char mask = 1 << bits;

    return (bray->bits.rawData[byte]&mask) >> bits;
}

unsigned char OBC_BitRayGet(OBC_BitRay *bray, size_t pos){
    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*char_BIT);
    const char mask = 1 << bits;

    return (bray->bits.rawData[byte]&mask) >> bits;
}
void OBC_BitRaySet(OBC_BitRay *bray, size_t pos, unsigned char bit){
    const size_t byte = pos/CHAR_BIT;
    const size_t bits = pos-(byte*char_BIT);
    const char mask = bit << bits;
    bray->bits.rawData[byte] &= ~mask;
    bray->bits.rawData[byte] ^= mask;
}

OBC_Offset OBC_BitRayFirstLeft0Bit(OBC_BitRay *bray, size_t pos){

    X_X_LONGEST *const bits = (X_X_LONGEST *)bray->bits.rawData;
    size_t i,j;

    for(i=0; i < bray->bits.maxUnitLength; i++){
        if(bits[i] != (~((X_X_LONGEST)0)) ){
            break;
        }
    }
    if(i == bray->bits.maxUnitLength){
        return OBC_NULL_INDEX;
    }

    i*=sizeof(X_X_LONGEST);
    bray->bits.rawData;
    for(j=0; j < sizeof(X_X_LONGEST); j++){
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


