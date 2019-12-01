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

    ///TODO maybe add basic init buffer
    /*
    size_t initAlloc =
        OBC_ALLOC_MAX_INIT_BUFFER_BYTES/unitSize;
    initAlloc = (initAlloc/2)*2;//round to 0 if large
    */

    /*
    if(OBC_initRay(
           OBC_TO_RAW_RAY(& allocator->backed.rawData)
           ,OBC_ALLOC_META_CHUNKSIZE/2,unitSize) == NULL){
        return NULL;
    }
    if(OBC_initRay(
            OBC_TO_RAW_RAY(& allocator->meta[0].rawData)
           , OBC_ALLOC_META_CHUNKSIZE/2,
       sizeof(OBC_ALLOC_META_TYPE)) == NULL){
        OBC_freeRayData(&allocator->backed.rawData);
        return NULL;
    }
    /*/
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
    //*/

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
    OBC_freeAllocator2Data(allocator);
    free(allocator_);
}

void OBC_freeAllocator2Data(void *allocator){
    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);
    OBC_freeRayData(&allocator_->backed);
    unsigned int i;
    for(i = 0; i <= allocator_->metaUnits; i++){
        OBC_freeRayData(& allocator_->meta[i]);
    }
}















//NEW_ALLOCAR

///TODO first is 0, therefore allocate +1
size_t OBC_metaFirst0Bit(OBC_ALLOC_META_TYPE *data, size_t unitsToCheck){

    size_t found = 0;

    size_t i;
    for(i=0; i < unitsToCheck; i++){
        /*
        OBC_ALLOC_META_TYPE cheq = data[i];
        cheq = ~cheq;
        if(cheq){
            break;
        }
        /*/
        if((OBC_ALLOC_META_TYPE)(~(data[i]))){
            break;
        }
        //*/
    }
    if(i==unitsToCheck){
        return OBC_NULL_INDEX;
    }

    found = i*OBC_ALLOC_META_BITS;

    //*
    char *raw = (char*)(&data[i]);

    ///printf("BYTES: %u \n",sizeof(OBC_ALLOC_META_TYPE)*OBC_ALLOC_META_CHUNKSIZE);

    size_t j;
    for(i = 0; i < sizeof(OBC_ALLOC_META_TYPE)*OBC_ALLOC_META_CHUNKSIZE; i++){
        if(~raw[i]){
            for(j=0; j < CHAR_BIT; j++){
                if(((raw[i]>>((CHAR_BIT-j)-1))&0x01) == 0){
                    return found+j+(i*CHAR_BIT);
                }
            }
            return OBC_NULL_INDEX;
        }
    }
    /*/

    OBC_ALLOC_META_TYPE *raw = data+i;

    size_t j,k;
    for(i = 0; i < OBC_ALLOC_META_CHUNKSIZE; i++){
        if((~(raw[i]))){
            for(j=0; j < OBC_ALLOC_META_BITS; j+=CHAR_BIT){
                #ifdef ENDIAN_BIG
                if(((raw[i]>>((OBC_ALLOC_META_BITS-j)-1))&0x01) == 0){
                    return found+j+(i*CHAR_BIT);
                }
                #else
                //int byteShift = ((j/CHAR_BIT)+1)*CHAR_BIT;
                //int bitSub = (j%CHAR_BIT)+1;
                //printf("RAW::: %p\n",raw[i]);
                //if(
                //   ((raw[i]>>(byteShift-bitSub))&0x1)
                //   == 0){
                //    return found+(byteShift-bitSub);
                //}
                if((~((raw[i]>>j)&0xFF))){
                    for(k=0; k < CHAR_BIT; k++){
                        if(((((raw[i]>>j)&0xFF)>>((CHAR_BIT-k)-1))&0x01) == 0){
                            return found+j+(k);
                        }
                    }
                }


                #endif
            }
            return found;
        }
    }

    //*/

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

        ///TODO MAY ERROR DUE TO BIT OVERRUN
        found = OBC_metaFirst0Bit(
            allocator_->meta[i].rawData+divPos,
            checkLen
            );
        if(found != OBC_NULL_INDEX
            ///TODO HOTFIX:
            || found < allocator_->backed.curUnitLength
            ){
            return OBC_ERROR_SUCCESS;
        }

        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;

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
                  allocator_->meta[index].rawData+(found/CHAR_BIT),
                  checkLen);

        if(place==OBC_NULL_INDEX){
            ///TODO ?
            ///critical error, some out of sync meta
            return OBC_NULL_INDEX;
        }

        found+=place;

    }

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
        if(place>0){
            OBC_Allocator2MarkAllocated3(allocator,place-1);
        }
    }

    if(place >= allocator_->backed.curUnitLength){
        place = allocator_->backed.curUnitLength;
        if(OBC_RayNewElement(& allocator_->backed.rawData) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

    }

    OBC_Allocator2MarkAllocated3(allocator,place);


    return place;

}

OBC_ERROR_ENUM OBC_Allocator2Free3(void *allocator, size_t pos){
    return OBC_Allocator2MarkFreed3(allocator,pos);
}

OBC_ERROR_ENUM OBC_Allocator2MarkFreed3(void *allocator, size_t pos){
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

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    char *mmeta = meta;
    mmeta[unit] |= unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit0__(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    unitMask=~unitMask;
    char *mmeta = meta;
    mmeta[unit] &= unitMask;

    return OBC_ERROR_NO_OP;
}
