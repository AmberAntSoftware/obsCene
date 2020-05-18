#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"


OBC_ERROR_ENUM OBC_metaMarkBit0(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));
    OBC_ALLOC_META_TYPE unitMask = 1<<((OBC_ALLOC_META_BITS-bits)-1);
    unitMask=~unitMask;
    meta[unit] &= unitMask;

    return OBC_ERROR_NO_OP;
}

/*
char OBC_metaGetBit(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/OBC_ALLOC_META_BITS;
    const size_t bits = (pos-(unit*OBC_ALLOC_META_BITS));

    return (char)(((meta[unit])>>bits)&1);
}
*/

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
           OBC_TO_RAY_PTR(& allocator->backed.rawData)
           ,0,unitSize) == OBC_ERROR_FAILURE){
        return NULL;
    }
    if(OBC_initRay(
            OBC_TO_RAY_PTR(& allocator->meta[0].rawData)
           , 1,
       sizeof(OBC_ALLOC_META_TYPE)) == OBC_ERROR_FAILURE){
        OBC_freeRayData(&allocator->backed);
        return NULL;
    }
    memset(allocator->meta[0].rawData,0,allocator->meta[0].maxUnitLength*allocator->meta[0].unitSize);

    unsigned int i;
    for(i = 1; i <= OBC_ALLOC_META_ADDRESSING; i++){
        if(OBC_initRay(
               OBC_TO_RAY_PTR(& allocator->meta[i].rawData)
               , 0,
               sizeof(OBC_ALLOC_META_TYPE)) == OBC_ERROR_FAILURE){
            while(i){
                OBC_freeRayData(& allocator->meta[i-1]);
                i--;
            }
            OBC_freeRayData(& allocator->backed);
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

size_t OBC_metaFirst0Bit(OBC_ALLOC_META_TYPE *data, size_t unitsToCheck){

    size_t found = 0;

    size_t i;
    for(i=0; i < unitsToCheck; i++){
        if((OBC_ALLOC_META_TYPE)(~(data[i]))){
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
                if(((raw[i]>>((CHAR_BIT-j)-1))&0x01) == 0){
                    return found+j+(i*CHAR_BIT);
                }
            }
            return OBC_NULL_INDEX;
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

        OBC_metaMarkBit1__((OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,pos);

        size_t divPos =
        (pos/OBC_ALLOC_META_CHUNKSIZE_BITS)*OBC_ALLOC_META_CHUNKSIZE;

        checkLen = allocator_->meta[i].maxUnitLength-divPos;
        if(checkLen < OBC_ALLOC_META_CHUNKSIZE){
            return OBC_ERROR_SUCCESS;
        }
        checkLen = OBC_ALLOC_META_CHUNKSIZE;

        found = OBC_metaFirst0Bit(
            (OBC_ALLOC_META_TYPE *)(allocator_->meta[i].rawData+divPos),
            checkLen
            );
        if(found != OBC_NULL_INDEX
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
        start = allocator_->meta[0].maxUnitLength*allocator_->meta[0].unitSize;
        if(OBC_RayExpand(& allocator_->meta[0].rawData) == OBC_ERROR_FAILURE){
            return OBC_ERROR_FAILURE;
        }
        memset(allocator_->meta[0].rawData+start,0,
                (allocator_->meta[0].maxUnitLength* allocator_->meta[0].unitSize)-start);
    }

    unsigned int i;
    for(i=1; i <= units; i++){

        curUnits=allocator_->meta[i-1].maxUnitLength;
        if((curUnits+1)/OBC_ALLOC_META_BITS >=
           allocator_->meta[i].maxUnitLength){
            start = allocator_->meta[i].maxUnitLength*allocator_->meta[i].unitSize;
            if(OBC_RayExpand(& allocator_->meta[i].rawData) == OBC_ERROR_FAILURE){
                debug("=========FAILURE: %i\n",i);
                return OBC_ERROR_FAILURE;
            }
            memset(allocator_->meta[i].rawData+start,0,
                (allocator_->meta[i].maxUnitLength*allocator_->meta[i].unitSize)-start);
        }
    }

    return OBC_ERROR_SUCCESS;

}

size_t OBC_Allocator2GetFreeLocation3(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    unsigned int units = allocator_->metaUnits;
    size_t found =
    OBC_metaFirst0Bit((OBC_ALLOC_META_TYPE *)allocator_->meta[units].rawData,
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
                  (OBC_ALLOC_META_TYPE *)(allocator_->meta[index].rawData+(found/CHAR_BIT)),
                  checkLen);

        if(place==OBC_NULL_INDEX){
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
        if(place>0){
            OBC_Allocator2MarkAllocated3(allocator,place-1);
        }
    }

    if(place >= allocator_->backed.curUnitLength){
        place = allocator_->backed.curUnitLength;
        if(OBC_RayNewElement(& allocator_->backed.rawData) == OBC_NULL_INDEX){
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
        OBC_metaMarkBit0__((OBC_ALLOC_META_TYPE *)allocator_->meta[i].rawData,pos);
        pos/=OBC_ALLOC_META_CHUNKSIZE_BITS;
    }

    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_metaMarkBit1__(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    unsigned char *mmeta = (unsigned char*)meta;
    mmeta[unit] |= unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit0__(OBC_ALLOC_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/CHAR_BIT;
    const size_t bits = (pos-(unit*(CHAR_BIT)));
    char unitMask = 1<<((CHAR_BIT-bits)-1);
    unitMask=~unitMask;
    unsigned char *mmeta = (unsigned char*)meta;
    mmeta[unit] &= unitMask;

    return OBC_ERROR_NO_OP;
}
