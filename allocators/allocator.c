#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"

void **OBC_newAllocRay(size_t unitSize){

    OBC_AllocRay *allocator = calloc(1,sizeof(OBC_AllocRay));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocRay(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

void *OBC_initAllocRay(void *allocator, size_t unitSize){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    if(OBC_initRay(&allocRay->backed,0,unitSize) == NULL){
        return NULL;
    }

    if(OBC_initRay(&allocRay->meta,0,sizeof(OBC_ALLOC_META_TYPE)) == NULL){
        OBC_freeRayData(&allocRay->backed);
        return NULL;
    }

    return allocRay;

}

void OBC_freeAllocRay(void *allocator){
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_freeAllocRayData(allocator);
    free(allocRay);
}

void OBC_freeAllocRayData(void *allocator){
    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_freeRayData(& allocRay->backed.rawData);
    OBC_freeRayData(& allocRay->meta.rawData);
}


size_t OBC_AllocRayMalloc(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    size_t place = OBC_AllocRayGetFreeLocation(allocator);
    if(place == OBC_NULL_INDEX){
        if(OBC_AllocRayExpand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        place = allocRay->backed.curUnitLength;
    }

    if(place >= allocRay->backed.curUnitLength){
        OBC_RayNewElement(& (allocRay->backed.rawData));
    }

    OBC_AllocRayMarkAllocated(allocator,place);

    return place;
}

OBC_ERROR_ENUM OBC_AllocRayFree(void *allocator, size_t data){

    OBC_AllocRayMarkFreed(allocator,data);

    return OBC_ERROR_NO_OP;

}

size_t OBC_AllocRayGetFreeLocation(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);

    size_t max = allocRay->meta.maxUnitLength;
    size_t i;
    size_t place = OBC_NULL_INDEX;

    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;
    for(i = 0; i < max; i++){
        if(raw[i]!= ~((OBC_ALLOC_META_TYPE)0)){
            break;
        }
    }

    if(i!=max){

        place = OBC_FindFirstEmptyBit(raw[i]);
        if(place==OBC_ALLOC_META_BITS){
            return OBC_NULL_INDEX;
        }
        place+=i*OBC_ALLOC_META_BITS;
        if(place >= allocRay->backed.maxUnitLength){
            return OBC_NULL_INDEX;
        }

    }else {
        return OBC_NULL_INDEX;
    }

    return place;

}

int OBC_FindFirstEmptyBit(OBC_ALLOC_META_TYPE rraw){

    int j;
    for(j = 0; j < OBC_ALLOC_META_BITS; j++){
        if( (rraw & OBC_ALLOC_META_MASK) == 0 ){
            break;
        }
        rraw<<=1;
    }
    return j;
}

OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t i;

    if(OBC_RayDoExpand(& allocRay->backed.rawData) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t metaMaxUnits = OBC_ALLOC_META_BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxLength;

        if(OBC_RayExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOC_MAX_CONTRACT_TRIES; i++){
                if(OBC_RayContract(& allocRay->backed.rawData) != OBC_ERROR_FAILURE){
                    return OBC_ERROR_FAILURE;
                }else{
                    break;
                }
            }
            return OBC_ERROR_FAILURE;
        }

        memset(allocRay->meta.rawData+start,0,((allocRay->meta.maxLength)-start));

    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_AllocRayMarkAllocated(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;
    size_t unit = pos/OBC_ALLOC_META_BITS;
    size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    raw[unit] = raw[unit] | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOC_META_TYPE *raw = (OBC_ALLOC_META_TYPE *)allocRay->meta.rawData;

    size_t unit = pos/OBC_ALLOC_META_BITS;
    size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    unitMask=~unitMask;

    raw[unit] = raw[unit] & unitMask;

    return OBC_ERROR_NO_OP;
}

































OBC_ERROR_ENUM OBC_metaMarkBit(OBC_ALLOC_META_TYPE *meta, const size_t pos, const char bitVal){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = (bitVal&1)<<((OBC_ALLOC_META_BITS-bits)-1);
    meta+=unit;
    *meta = ((*meta) & (~unitMask))|unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit1(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    meta[unit] |= unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit0(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    unitMask=~unitMask;
    meta[unit] &= unitMask;

    return OBC_ERROR_NO_OP;
}

char OBC_metaGetBit(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));

    return (char)(((meta[unit])>>bits)&1);
}



size_t OBC_metaFirstEmptyBit(OBC_ALLOC_META_TYPE *meta, size_t metaUnitLength){

    return OBC_metaFirst0Bit(meta,metaUnitLength);

    size_t i;

    for(i = 0; i < metaUnitLength; i++){
        if(meta[i] != ~0){
            break;
        }
    }

    if(i==metaUnitLength){
        return OBC_NULL_INDEX;
    }

    /*
    OBC_ALLOC_META_TYPE raw = meta[i];
    size_t j;
    for(j = 0; j < OBC_ALLOC_META_BITS; j++){
        if( ((raw<<j) & OBC_ALLOC_META_MASK) == 0 ){
            return (i*OBC_ALLOC_META_BITS)+j;
        }
    }
    /*/

    char *raw = (meta+i);

    const char C_BIT = 1<<(CHAR_BIT-1);

    size_t j,k;
    for(j = 0; j < sizeof(OBC_ALLOC_META_TYPE); j++){
        char rraw = raw[j];
        for(k = 0; k < CHAR_BIT; k++){
            char rax = ((rraw<<k)&C_BIT);
            if(rax == 0){
                size_t ret = (i*OBC_ALLOC_META_BITS)+(j*CHAR_BIT)+k;
                return ret;
            }
        }
    }
    //*/

    return OBC_NULL_INDEX;
}



























void **OBC_newAllocator(size_t unitSize){

    OBC_Allocator *allocator = calloc(1,sizeof(OBC_Allocator));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocator(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->rawAlloc.backed.rawData;

}

void *OBC_initAllocator(void *allocator, size_t unitSize){

    OBC_Allocator *allocator_ = OBC_TO_RAW_ALLOCATOR(allocator);

    if(OBC_initAllocRay(&allocator_->rawAlloc,unitSize) == NULL){
        return NULL;
    }

    unsigned int i;
    for(i = 0; i < OBC_ALLOC_META_ADDRESSING; i++){
        if(OBC_initRay(& allocator_->meta[i], 0,
                       sizeof(OBC_ALLOC_META_TYPE)) == NULL){
            while(i){
                OBC_freeRayData(& allocator_->meta[i-1]);
                i--;
            }
            OBC_freeAllocRayData(& allocator_->rawAlloc);
            return NULL;
        }
    }

    allocator_->metaUnits = 0;

    return allocator_;

}

void OBC_freeAllocator(void *allocator){
    OBC_Allocator *allocator_ = OBC_TO_RAW_ALLOCATOR(allocator);
    OBC_freeAllocatorData(allocator_);
    free(allocator_);
}

void OBC_freeAllocatorData(void *allocator){
    OBC_Allocator *allocator_ = OBC_TO_RAW_ALLOCATOR(allocator);
    OBC_freeAllocRayData(&allocator_->rawAlloc);
    unsigned int i;
    for(i = 0; i < OBC_ALLOC_META_ADDRESSING; i++){
        OBC_freeRayData(& allocator_->meta[i]);
    }
}


















void **OBC_newAllocator2(size_t unitSize){

    OBC_Allocator2 *allocator = calloc(1,sizeof(OBC_Allocator2));

    if(allocator == NULL){
        return NULL;
    }

    if(OBC_initAllocator2(allocator, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

void *OBC_initAllocator2(OBC_Allocator2 *allocator, size_t unitSize){

    if(OBC_initRay(
           OBC_TO_RAW_RAY(& allocator->backed.rawData)
           ,0,unitSize) == NULL){
        return NULL;
    }
    if(OBC_initRay(
            OBC_TO_RAW_RAY(& allocator->meta[0].rawData)
           , 1,
       sizeof(OBC_ALLOC_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->backed.rawData);
        return NULL;
    }
    memset(allocator->meta[0].rawData,0,allocator->meta[0].maxLength);

    unsigned int i;
    for(i = 1; i <= OBC_ALLOC_META_ADDRESSING; i++){
        if(OBC_initRay(
               OBC_TO_RAW_RAY(& allocator->meta[i].rawData)
               , 0,
               sizeof(OBC_ALLOC_META_TYPE)) == NULL){
            while(i){
                OBC_freeRayData(& allocator->meta[i-1]);
                i--;
            }
            OBC_freeAllocRayData(& allocator->backed);
            return NULL;
        }
    }

    allocator->metaUnits = 0;

    return allocator;

}

void OBC_freeAllocator2(void *allocator){
    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);
    OBC_freeAllocator2Data(allocator_);
    free(allocator_);
}

void OBC_freeAllocator2Data(void *allocator){
    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);
    OBC_freeAllocRayData(&allocator_->backed);
    unsigned int i;
    for(i = 0; i <= allocator_->metaUnits; i++){
        OBC_freeRayData(& allocator_->meta[i]);
    }
}

OBC_ERROR_ENUM OBC_Allocator2Free(void *allocator, size_t pos){
    return OBC_Allocator2MarkFreed(allocator,pos);
}

size_t OBC_Allocator2Malloc(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    /*
    size_t place = OBC_Allocator2GetFreeLocation(allocator);
    /*/
    size_t place = OBC_Allocator2GetFreeLocation2(allocator);
    //*/
    if(place == OBC_NULL_INDEX){

        if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        place = allocator_->backed.curUnitLength;

    }else{

        if(place >= allocator_->backed.curUnitLength){
            if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
                return OBC_NULL_INDEX;
            }
            if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
                return OBC_NULL_INDEX;
            }
        }

    }

    OBC_Allocator2MarkAllocated2(allocator,place);

    return place;
}

size_t OBC_Allocator2GetFreeLocation(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i, index;

    size_t freeLocation = 0, found = 0;
    size_t maxLen = 0;

    if(units == 0){
        found = OBC_metaFirstEmptyBit(
                     (OBC_ALLOC_META_TYPE *)allocator_->meta[0].rawData,
                     allocator_->meta[0].maxUnitLength);
        if(found > allocator_->backed.maxUnitLength){
            found = OBC_NULL_INDEX;
        }
        return found;
    }

    for(i = 0; i <= units; i++){
        index = (units-i);

        ///TODO decide if unnecessary as any real
        ///will be crimped due to existing lenth
        maxLen = allocator_->meta[index].maxUnitLength
            -(freeLocation/OBC_ALLOC_META_BITS);

        if(maxLen > OBC_ALLOC_META_CHUNKSIZE){
            maxLen = OBC_ALLOC_META_CHUNKSIZE;
        }

        found = OBC_metaFirstEmptyBit(
            (OBC_ALLOC_META_TYPE *)allocator_->meta[index].rawData
                          +(freeLocation/OBC_ALLOC_META_BITS),
            maxLen
        );
        if(found == OBC_NULL_INDEX){
            return OBC_NULL_INDEX;
        }

        freeLocation =
        freeLocation*OBC_ALLOC_META_CHUNKSIZE_BITS
        +found;

    }

    return freeLocation;

}

OBC_ERROR_ENUM OBC_Allocator2Expand(void *allocator){

    const size_t expansionLimits[OBC_ALLOC_META_ADDRESSING] =
        OBC_ALLOC_META_LIMITS;

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    unsigned int i;

    /**
    if(OBC_RayExpand(& allocator_->backed.rawData)==OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    //**/

    if(units<OBC_ALLOC_META_ADDRESSING){
        if((allocator_->backed.curUnitLength+1)
           >= expansionLimits[units] && expansionLimits[units] != 0){
            if(OBC_RayExpand(& allocator_->meta[units+1].rawData) != OBC_ERROR_FAILURE){
                units++;
                allocator_->metaUnits = units;
            }
        }
    }

    size_t start = 0;

    /// ////////////////////
    /*
    size_t start = 0;
    if(allocator_->backed.curUnitLength >=
       allocator_->meta[0].maxUnitLength*OBC_ALLOC_META_BITS){
        start = allocator_->meta[0].maxLength;
        if(OBC_RayExpand(& (allocator_->meta[0].rawData))==OBC_ERROR_FAILURE){
            return OBC_ERROR_FAILURE;
        }
        memset(allocator_->meta[0].rawData+start,0,
               ((allocator_->meta[0].maxLength)-start));
    }
    */
    /// ////////////////////

    size_t prevSize = allocator_->backed.curUnitLength;

    char resized = 0;
    for(i = 0; i <= units; i++){
        if(prevSize >= allocator_->meta[i].maxUnitLength*OBC_ALLOC_META_BITS){

            start = allocator_->meta[i].maxLength;
            if(OBC_RayExpand(& allocator_->meta[i].rawData)==OBC_ERROR_FAILURE){
                ///TODO contract loop
                return OBC_ERROR_FAILURE;
            }
            memset(allocator_->meta[i].rawData+start,0,((allocator_->meta[i].maxLength)-start));

            prevSize/=OBC_ALLOC_META_CHUNKSIZE_BITS;
            resized = 1;
            continue;
        }
        break;
    }

    if(!resized){
        return OBC_ERROR_SUCCESS;
    }
    /**
    i--;
    while(i <= OBC_ALLOC_META_ADDRESSING+1){
        size_t start = (allocator_->meta[i].maxUnitLength/2)*sizeof(OBC_ALLOC_META_TYPE);
        memset(allocator_->meta[i].rawData+start,0,((allocator_->meta[i].maxLength)-start));
        i--;
    }
    //*/

    /**
    i=0;
    do{

        i++;
    }while(i < units);
    **/

    /*
    i = 0;
    do{

        if(OBC_RayExpand(& allocator_->meta[i].rawData)==OBC_ERROR_FAILURE){

            do{

                for(j = 0;
                    OBC_RayContract(& allocator_->meta[i-1].rawData)==OBC_ERROR_FAILURE
                    && j < OBC_ALLOC_MAX_CONTRACT_TRIES; j++ ){
                    ///something horrible happened, continue to attempt other trimming
                }

                i--;
            }while(i);

            return OBC_ERROR_FAILURE;
        }

        prevSize = allocator_->meta[i-1].maxUnitLength;

        i++;
    }while(i < units);

    i=0;
    do{
        size_t start = allocator_->meta[i].maxLength/2;
        memset(allocator_->meta[i].rawData+start,0,((allocator_->meta[i].maxLength)-start));
        i++;
    }while(i < units);
    */

    return OBC_ERROR_SUCCESS;

}

OBC_ERROR_ENUM OBC_Allocator2MarkAllocated(void *allocator, size_t pos){

    //single threaded so only edge allocations cause upper full bits

    const OBC_Allocator2 *const allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[0].rawData,
                 pos);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i;
    for(i = 1; i <= units; i++){

        if(pos == 0 || pos%(OBC_ALLOC_META_CHUNKSIZE_BITS) != 0){
            return OBC_ERROR_SUCCESS;
        }
        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;

        OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
                 pos);

    }

    return OBC_ERROR_SUCCESS;

}

OBC_ERROR_ENUM OBC_Allocator2MarkFreed(void *allocator, size_t pos){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;
    unsigned int i;
    for(i = 0; i <= units; i++){

        OBC_metaMarkBit0(
            (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
             pos
        );

        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;
    }

    return OBC_ERROR_SUCCESS;
}


size_t OBC_Allocator2GetFreeLocation2(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i, index;

    size_t freeLocation = 0, found = 0;
    size_t maxLen = 0;

    maxLen = allocator_->meta[units].maxUnitLength;
    freeLocation = OBC_metaFirstEmptyBit(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[units].rawData,
        maxLen);

    if(freeLocation == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    for(i = 0; i < units; i++){
        index = (units-1)-i;

        freeLocation*=OBC_ALLOC_META_CHUNKSIZE_BITS;

        maxLen = allocator_->meta[index].maxUnitLength-freeLocation;
        if(maxLen > OBC_ALLOC_META_CHUNKSIZE){
            maxLen = OBC_ALLOC_META_CHUNKSIZE;
        }

        found = OBC_metaFirstEmptyBit(
        (OBC_ALLOC_META_TYPE *)
            allocator_->meta[index].rawData+freeLocation,
        maxLen);

        if(found == OBC_NULL_INDEX
           || freeLocation+found >= allocator_->backed.curUnitLength
           ){
            return OBC_NULL_INDEX;
        }

        freeLocation += found;

    }

    /*
    if(units == 0){
        found = OBC_metaFirstEmptyBit(
                     (OBC_ALLOC_META_TYPE *)allocator_->meta[0].rawData,
                     allocator_->meta[0].maxUnitLength);
        if(found > allocator_->backed.maxUnitLength){
            found = OBC_NULL_INDEX;
        }
        return found;
    }

    for(i = 0; i <= units; i++){
        index = (units-i);

        ///TODO decide if unnecessary as any real
        ///will be crimped due to existing lenth
        maxLen = allocator_->meta[index].maxUnitLength
            -(freeLocation/OBC_ALLOC_META_BITS);

        if(maxLen > OBC_ALLOC_META_CHUNKSIZE){
            maxLen = OBC_ALLOC_META_CHUNKSIZE;
        }

        found = OBC_metaFirstEmptyBit(
            (OBC_ALLOC_META_TYPE *)allocator_->meta[index].rawData
                          +(freeLocation/OBC_ALLOC_META_BITS),
            maxLen
        );
        if(found == OBC_NULL_INDEX){
            return OBC_NULL_INDEX;
        }

        freeLocation =
        freeLocation*OBC_ALLOC_META_CHUNKSIZE_BITS
        +found;

    }*/

    return freeLocation;

}

/*
OBC_ERROR_ENUM OBC_Allocator2MarkAllocated2(void *allocator, size_t pos){

    //single threaded so only edge allocations cause upper full bits

    const OBC_Allocator2 *const allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[0].rawData,
                 pos);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i;
    for(i = 1; i <= units; i++){

        if(pos == 0 || pos%(OBC_ALLOC_META_CHUNKSIZE_BITS) != 0){
            return OBC_ERROR_SUCCESS;
        }
        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;

        OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
                 pos);

    }

    return OBC_ERROR_SUCCESS;

}
*/

//*
OBC_ERROR_ENUM OBC_Allocator2MarkAllocated2(void *allocator, size_t pos){

    //single threaded so only edge allocations cause upper full bits

    const OBC_Allocator2 *const allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i,j;
    for(i = 0; i <= units; i++){

        OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
                 pos);

        ///TODO COMPLETE
        size_t vc = 0;
        size_t posDiv = pos/OBC_ALLOC_META_CHUNKSIZE_BITS;
        for(vc = posDiv;
            vc < posDiv+OBC_ALLOC_META_CHUNKSIZE;
            vc++){
            char rraw = allocator_->meta[i].rawData[vc];
            if(rraw != (char)(~0)){
                return OBC_ERROR_SUCCESS;
            }
        }

        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;

        /*OBC_metaMarkBit1(
        (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
                 pos);*/

    }

    return OBC_ERROR_SUCCESS;

}
//*/

























OBC_ERROR_ENUM OBC_Allocator2Expand2(void *allocator){

    const size_t expansionLimits[OBC_ALLOC_META_ADDRESSING] =
        OBC_ALLOC_META_LIMITS;

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    unsigned int i;

    if(units<OBC_ALLOC_META_ADDRESSING){
        if((allocator_->backed.curUnitLength+1)
           >= expansionLimits[units] && expansionLimits[units] != 0){
            if(OBC_RayExpand(& allocator_->meta[units+1].rawData) != OBC_ERROR_FAILURE){
                units++;
                allocator_->metaUnits = units;
            }
        }
    }

    size_t start = 0;

    size_t prevSize = allocator_->backed.curUnitLength;

    char resized = 0;
    for(i = 0; i <= units; i++){
        if(prevSize >= allocator_->meta[i].maxUnitLength*OBC_ALLOC_META_BITS){

            start = allocator_->meta[i].maxLength;
            if(OBC_RayExpand(& allocator_->meta[i].rawData)==OBC_ERROR_FAILURE){
                ///TODO contract loop
                return OBC_ERROR_FAILURE;
            }
            memset(allocator_->meta[i].rawData+start,0,((allocator_->meta[i].maxLength)-start));

            prevSize/=OBC_ALLOC_META_CHUNKSIZE_BITS;
            resized = 1;
            continue;
        }
        break;
    }

    if(!resized){
        return OBC_ERROR_SUCCESS;
    }

    return OBC_ERROR_SUCCESS;

}

size_t OBC_Allocator2Malloc2(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    /*
    size_t place = OBC_Allocator2GetFreeLocation(allocator);
    /*/
    if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    size_t place = OBC_metaFirstEmptyBit(
                    allocator_->meta[0].rawData,
                    allocator_->meta[0].maxUnitLength
                    );


    if(place == OBC_NULL_INDEX){
        place = allocator_->backed.curUnitLength;
    }
    if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
        return OBC_NULL_INDEX;
    }
    if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    //*/
    /*
    if(place == OBC_NULL_INDEX){

        if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        place = allocator_->backed.curUnitLength;

    }else{

        if(place >= allocator_->backed.curUnitLength){
            if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
                return OBC_NULL_INDEX;
            }
            if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
                return OBC_NULL_INDEX;
            }
        }

    }*/

    OBC_Allocator2MarkAllocated2(allocator,place);

    return place;
}






































































//NEW_ALLOCAR

///TODO first is 0, therefore allocate +1
size_t OBC_metaFirst0Bit(OBC_ALLOC_META_TYPE *data, size_t unitsToCheck){

    size_t found = 0;

    size_t i;
    for(i=0; i < unitsToCheck; i++){
        OBC_ALLOC_META_TYPE cheq = data[i];
        cheq = ~cheq;
        if(cheq){
            break;
        }
    }
    if(i==unitsToCheck){
        return OBC_NULL_INDEX;
    }

    found = i*OBC_ALLOC_META_BITS;

    char *raw = (char*)(&data[i]);

    size_t j;
    for(i = 0; i < sizeof(OBC_ALLOC_META_TYPE)*OBC_ALLOC_META_CHUNKSIZE; i++){
        if(~raw[i]){
            for(j=0; j < CHAR_BIT; j++){
                if(((raw[i]>>((CHAR_BIT-j)-1))&1) == 0){
                    return found+j+(i*CHAR_BIT);
                }
            }
            return found;
        }
    }

    return OBC_NULL_INDEX;

}

OBC_ERROR_ENUM OBC_Allocator2MarkAllocated3(void *allocator, size_t pos){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;

    unsigned int i;
    size_t checkLen,found ;
    for(i=0;i<=units;i++){

        OBC_metaMarkBit1__(allocator_->meta[i].rawData,pos);

        size_t divPos =
        (pos/OBC_ALLOC_META_CHUNKSIZE_BITS)*OBC_ALLOC_META_CHUNKSIZE;

        checkLen = allocator_->meta[i].maxUnitLength-divPos;
        if(checkLen < OBC_ALLOC_META_CHUNKSIZE){
            return OBC_ERROR_SUCCESS;
        }
        checkLen = OBC_ALLOC_META_CHUNKSIZE;

//return OBC_ERROR_SUCCESS;
        ///TODO MAY ERROR DUE TO BIT OVERRUN
        found = OBC_metaFirst0Bit(
            allocator_->meta[i].rawData+divPos,
            checkLen
            );
        if(found != OBC_NULL_INDEX
            ///TODO HOTFIX:
            || found < allocator_->backed.curUnitLength
            ){
            debug("///ESCAPE:: i:%u    check:%u\n",i,checkLen);
            //non null therefore available, therefore return
            return OBC_ERROR_SUCCESS;
        }

        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;
///printf(">>FOUND:: %u\n",found);
///printf(">>__UNITS:: %u\n",allocator_->metaUnits);
    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_Allocator2Expand3(void *allocator){

    const size_t expansionLimits[OBC_ALLOC_META_ADDRESSING] =
        OBC_ALLOC_META_LIMITS;

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    size_t curUnits = allocator_->backed.curUnitLength;
    size_t start;

    if(units < OBC_ALLOC_META_ADDRESSING){
        if(curUnits+1 >= expansionLimits[units]
           && expansionLimits[units] != 0){
            units++;
            allocator_->metaUnits = units;
                /*tart = allocator_->meta[units].maxLength;
                if(OBC_RayExpand(& allocator_->meta[units].rawData) == OBC_ERROR_FAILURE){
                    ///TODO handle error
                    return OBC_ERROR_FAILURE;
                }
                memset(allocator_->meta[units].rawData+start,0,
                allocator_->meta[units].maxLength-start);
                //allocator_->meta[units].rawData[0] = 1<<(CHAR_BIT-1);
                */
        }
    }

    if((curUnits+1)/OBC_ALLOC_META_BITS >=
       allocator_->meta[0].maxUnitLength){
        start = allocator_->meta[0].maxLength;
        if(OBC_RayExpand(& allocator_->meta[0].rawData) == OBC_ERROR_FAILURE){
            ///TODO CONTRACT
            return OBC_ERROR_FAILURE;
        }
        memset(allocator_->meta[0].rawData+start,0,
                allocator_->meta[0].maxLength-start);
    }

    unsigned int i;
    for(i=1; i <= units; i++){

        curUnits=allocator_->meta[i-1].maxUnitLength;
        if((curUnits+1)/OBC_ALLOC_META_BITS >=
           allocator_->meta[i].maxUnitLength){
            start = allocator_->meta[i].maxLength;
            if(OBC_RayExpand(& allocator_->meta[i].rawData) == OBC_ERROR_FAILURE){
                ///TODO CONTRACT
                debug("=========FAILURE: %i\n",i);
                return OBC_ERROR_FAILURE;
            }
            memset(allocator_->meta[i].rawData+start,0,
                allocator_->meta[i].maxLength-start);
        }
    }

    return OBC_ERROR_SUCCESS;

}

size_t OBC_Allocator2GetFreeLocation3(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    size_t found =
    OBC_metaFirst0Bit(allocator_->meta[units].rawData,
                      allocator_->meta[units].maxUnitLength);
    size_t place = 0;

    debug("---units:: %u\n",units);
    debug("///INIT_FOUND:: %u\n",found);

    size_t checkLen;
    unsigned int i,index;
    for(i=0; i < units; i++){
        index = units-i-1;


        found*=OBC_ALLOC_META_CHUNKSIZE_BITS;

        checkLen = allocator_->meta[index].maxUnitLength-(found/OBC_ALLOC_META_BITS);
        if(checkLen > OBC_ALLOC_META_CHUNKSIZE){
            checkLen = OBC_ALLOC_META_CHUNKSIZE;
        }

        place = OBC_metaFirst0Bit(
                  allocator_->meta[index].rawData+(found/OBC_ALLOC_META_BITS),
                  checkLen);

        debug("///NEXT_FROM:: %i\n",(int)((allocator_->meta[0].rawData[0])));
        debug("///NEXT_FOUND:: %u\n",place);

        if(place==OBC_NULL_INDEX){
            ///critical error, some out of sync meta
            return OBC_NULL_INDEX;
        }
        debug("|||||PLACE_POS:: %u\n",place);

        found+=place;

    }

    debug("|||||FOUND_POS:: %u\n",found);

    return found;
}

size_t OBC_Allocator2Malloc3(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    size_t place = OBC_Allocator2GetFreeLocation3(allocator);
    if(place==OBC_NULL_INDEX){
        if(OBC_Allocator2Expand3(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
        place = allocator_->backed.curUnitLength;
        //mark previous
        OBC_Allocator2MarkAllocated3(allocator,place-1);
    }

    if(place==OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    if(place >= allocator_->backed.curUnitLength){
        if(OBC_RayNewElement(& allocator_->backed.rawData) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    OBC_Allocator2MarkAllocated3(allocator,place);

    return place;

}

OBC_ERROR_ENUM OBC_Allocator2Free3(void *allocator, size_t pos){
    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    unsigned int i;
    for(i=0; i <= units; i++){
        OBC_metaMarkBit0__(allocator_->meta[i].rawData,pos);
        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;
    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_metaMarkBit1__(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/((sizeof(OBC_ALLOC_META_TYPE)*CHAR_BIT));
    const size_t bits = (pos-(unit*(sizeof(OBC_ALLOC_META_TYPE)*CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    char *mmeta = meta;
    mmeta[unit] |= unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit0__(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/(OBC_ALLOC_META_BITS*sizeof(OBC_ALLOC_META_TYPE));
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS*sizeof(OBC_ALLOC_META_TYPE)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    unitMask=~unitMask;
    char *mmeta = meta;
    mmeta[unit] &= unitMask;

    return OBC_ERROR_NO_OP;
}
