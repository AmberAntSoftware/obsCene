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

    bray->bitcount = 0;

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

char OBC_BitRayGet(OBC_BitRay *bray, size_t pos){
    //
}

char OBC_BitRaySet(OBC_BitRay *bray, size_t pos){
    //
}

OBC_ERROR_ENUM OBC_BitRayDoExpand(OBC_BitRay *bray, size_t bitPosNeeded){
    if(bray->bits.maxUnitLength*OBC_BITRAY_META_BITS <= bitPosNeeded){
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
