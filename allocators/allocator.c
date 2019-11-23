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

    size_t i;
    for(i = 0; i < metaUnitLength; i++){
        if(meta[i] != (~((OBC_ALLOC_META_TYPE)(0)))){
            break;
        }
    }

    if(i==metaUnitLength){
        return OBC_NULL_INDEX;
    }

    OBC_ALLOC_META_TYPE raw = *meta;

    size_t j;
    for(j = 0; j < OBC_ALLOC_META_BITS; j++){
        if( ((raw<<j) & OBC_ALLOC_META_MASK) == 0 ){
            return (i*sizeof(OBC_ALLOC_META_TYPE))+j;
        }
    }

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

    if(OBC_initAllocator2(& allocator->backed.rawData, unitSize) == NULL){
        free(allocator);
        return NULL;
    }

    return (void**)&allocator->backed.rawData;

}

void *OBC_initAllocator2(void *allocator, size_t unitSize){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    if(OBC_initAllocRay(& allocator_->backed.rawData,unitSize) == NULL){
        return NULL;
    }
    if(OBC_initRay(& allocator_->meta[0].rawData, 1,
       sizeof(OBC_ALLOC_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator_->backed.rawData);
        return NULL;
    }
    memset(allocator_->meta[0].rawData,0,allocator_->meta[0].maxLength);
    unsigned int i;
    for(i = 1; i <= OBC_ALLOC_META_ADDRESSING; i++){
        if(OBC_initRay(& allocator_->meta[i].rawData, 0,
                       sizeof(OBC_ALLOC_META_TYPE)) == NULL){
            while(i){
                OBC_freeRayData(& allocator_->meta[i-1]);
                i--;
            }
            OBC_freeAllocRayData(& allocator_->backed);
            return NULL;
        }
    }

    allocator_->metaUnits = 0;

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

size_t OBC_Allocator2Malloc(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    size_t place = OBC_Allocator2GetFreeLocation(allocator);
    if(place == OBC_NULL_INDEX){
        if(OBC_Allocator2Expand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
        place = allocator_->backed.curUnitLength;

        ///place = allocator_->rawAlloc.backed.curUnitLength;
    }

    if(place >= allocator_->backed.curUnitLength){
        if(OBC_RayNewElement(& (allocator_->backed.rawData)) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }
    ///printf("VAL: %i\n",((OBC_ALLOC_META_TYPE*)allocator_->meta[0].rawData)[place/OBC_ALLOC_META_BITS]);
    OBC_Allocator2MarkAllocated(allocator,place);
    //printf("VAL_U: %i\n",((OBC_ALLOC_META_TYPE*)allocator_->meta[0].rawData)[place/OBC_ALLOC_META_BITS]);

    ///if(place >= allocator_->rawAlloc.backed.curUnitLength){
    ///    OBC_RayNewElement(& (allocator_->rawAlloc.backed.rawData));
    ///}

    ///OBC_AllocRayMarkAllocated(allocator,place);

    return place;
}

size_t OBC_Allocator2GetFreeLocation(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);


    /*int a = 0;
    if(a==0){
        return OBC_metaFirstEmptyBit(
                 (OBC_ALLOC_META_TYPE *)allocator_->meta[0].rawData,
                 allocator_->meta[0].maxUnitLength);
    }*/

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
    }else{

        for(i = 0; i <= units; i++){
            index = (units-i);

            ///TODO decide if unnecessary as any real
            ///will be crimped due to existing lenth
            maxLen = allocator_->meta[index].maxUnitLength-(freeLocation/OBC_ALLOC_META_BITS);

            if(maxLen > OBC_ALLOC_META_CHUNKSIZE){
                maxLen = OBC_ALLOC_META_CHUNKSIZE;
            }

            found = OBC_metaFirstEmptyBit(
                (OBC_ALLOC_META_TYPE *)allocator_->meta[index].rawData+(freeLocation/OBC_ALLOC_META_BITS),
                maxLen
            );
            if(found == OBC_NULL_INDEX){
                return OBC_NULL_INDEX;
            }

            freeLocation =
            freeLocation*OBC_ALLOC_META_CHUNKSIZE_BITS
            +found;

        }

    }

    return freeLocation;

}

OBC_ERROR_ENUM OBC_Allocator2Expand(void *allocator){

    const size_t expansionLimits[OBC_ALLOC_META_ADDRESSING] =
        {OBC_ALLOC_UNITS_0,OBC_ALLOC_UNITS_1,
        OBC_ALLOC_UNITS_2,OBC_ALLOC_UNITS_3};

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    unsigned int i;

    /**
    if(OBC_RayExpand(& allocator_->backed.rawData)==OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
    //**/

    if(units<4){
        if(allocator_->backed.curLength >= expansionLimits[units]){
            units++;
            allocator_->metaUnits = units;
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

    const OBC_Allocator2 *const allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;
    unsigned int i;
    for(i = 0; i < units; i++){

        OBC_metaMarkBit0(
            (OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,
             pos
        );

        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;
    }

    return OBC_ERROR_SUCCESS;
}
