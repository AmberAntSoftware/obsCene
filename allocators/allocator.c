#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"

/*
static const size_t UNITS_0 = OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS;
static const size_t UNITS_1 = OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_0;
static const size_t UNITS_2 = OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_1;
static const size_t UNITS_3 = OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS*UNITS_2;
*/

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

    if(OBC_initRay(&allocRay->meta,0,sizeof(OBC_ALLOCATOR_META_TYPE)) == NULL){
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

    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    for(i = 0; i < max; i++){
        if(raw[i]!= ~((OBC_ALLOCATOR_META_TYPE)0)){
            break;
        }
    }

    if(i!=max){

        place = OBC_FindFirstEmptyBit(raw[i]);
        if(place==META_BITS){
            return OBC_NULL_INDEX;
        }
        place+=i*META_BITS;
        if(place >= allocRay->backed.maxUnitLength){
            return OBC_NULL_INDEX;
        }

    }else {
        return OBC_NULL_INDEX;
    }

    return place;

}

int OBC_FindFirstEmptyBit(OBC_ALLOCATOR_META_TYPE rraw){

    int j;
    for(j = 0; j < META_BITS; j++){
        if( (rraw & META_MASK) == 0 ){
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

    const size_t metaMaxUnits = META_BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxLength;

        if(OBC_RayExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOCATOR_MAX_CONTRACT_TRIES; i++){
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
    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    size_t unit = pos/META_BITS;
    size_t bits = (pos-(unit*META_BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((META_BITS-bits)-1);
    raw[unit] = raw[unit] | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocRayMarkFreed(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;

    size_t unit = pos/META_BITS;
    size_t bits = (pos-(unit*META_BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((META_BITS-bits)-1);
    unitMask=~unitMask;

    raw[unit] = raw[unit] & unitMask;

    return OBC_ERROR_NO_OP;
}

































OBC_ERROR_ENUM OBC_metaMarkBit(OBC_ALLOCATOR_META_TYPE *meta, const size_t pos, const char bitVal){

    const size_t unit = pos/META_BITS;
    const size_t bits = (pos-(unit*META_BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = (bitVal&1)<<((META_BITS-bits)-1);
    meta+=unit;
    *meta = ((*meta) & (~unitMask))|unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit1(OBC_ALLOCATOR_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/META_BITS;
    const size_t bits = (pos-(unit*META_BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((META_BITS-bits)-1);
    meta+=unit;
    *meta = (*meta) | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_metaMarkBit0(OBC_ALLOCATOR_META_TYPE *meta, const size_t pos){

    const size_t unit = pos/META_BITS;
    const size_t bits = (pos-(unit*META_BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((META_BITS-bits)-1);
    unitMask=~unitMask;
    meta+=unit;
    *meta = (*meta) & unitMask;

    return OBC_ERROR_NO_OP;
}




size_t OBC_metaFirstEmptyBit(OBC_ALLOCATOR_META_TYPE *meta, size_t metaUnitLength){

    size_t i;
    for(i = 0; i < metaUnitLength; i++){
        if(*meta != ~((OBC_ALLOCATOR_META_TYPE)(0))){
            break;
        }
        meta++;
    }

    if(i==metaUnitLength){
        return OBC_NULL_INDEX;
    }

    OBC_ALLOCATOR_META_TYPE raw = *meta;

    size_t j;
    for(j = 0; j < (sizeof(OBC_ALLOCATOR_META_TYPE)<<3); j++){
        if( (raw & META_MASK) == 0 ){
            break;
        }
        raw<<=1;
    }

    if(j==(sizeof(OBC_ALLOCATOR_META_TYPE)<<3)){
        return OBC_NULL_INDEX;
    }

    return (i*sizeof(OBC_ALLOCATOR_META_TYPE))+j;
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
    for(i = 0; i < OBC_ALLOCATOR_META_ADDRESSING; i++){
        if(OBC_initRay(& allocator_->meta[i], 0,
                       sizeof(OBC_ALLOCATOR_META_TYPE)) == NULL){
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
    for(i = 0; i < OBC_ALLOCATOR_META_ADDRESSING; i++){
        OBC_freeRayData(& allocator_->meta[i]);
    }
}

size_t OBC_AllocatorMalloc(void *allocator){

    OBC_Allocator *allocator_ = OBC_TO_RAW_ALLOCATOR(allocator);

    size_t place = OBC_AllocatorGetFreeLocation(allocator);
    if(place == OBC_NULL_INDEX){
        if(OBC_AllocRayExpand(allocator) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }

        place = allocator_->rawAlloc.backed.curUnitLength;
    }

    if(place >= allocator_->rawAlloc.backed.curUnitLength){
        OBC_RayNewElement(& (allocator_->rawAlloc.backed.rawData));
    }

    OBC_AllocRayMarkAllocated(allocator,place);

    return place;
}
/*
size_t OBC_AllocatorMalloc(void *allocator){

    OBC_Allocator *allocRay = OBC_TO_RAW_ALLOCATOR(allocator);

    size_t place = OBC_AllocatorGetFreeLocation(allocator);
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
*/

size_t OBC_AllocatorGetFreeLocation(void *allocator){

    OBC_Allocator *alloc = OBC_TO_RAW_ALLOCATOR(allocator);

    const unsigned int units = alloc->metaUnits;

    unsigned int i, index;

    size_t freeLocation = 0, found = 0;

    if(units == 0){
        found = OBC_metaFirstEmptyBit(
                     (OBC_ALLOCATOR_META_TYPE *)alloc->rawAlloc.meta.rawData,
                     alloc->rawAlloc.meta.maxUnitLength);
        return found;
    }else{

        for(i = 0; i < units; i++){
            index = (units-i)-1;

            freeLocation = freeLocation
            *(sizeof(OBC_ALLOCATOR_META_TYPE)*8)
            *OBC_ALLOCATOR_META_CHUNKSIZE+found;

            found = OBC_metaFirstEmptyBit(
                     (OBC_ALLOCATOR_META_TYPE *)alloc->meta[index].rawData,
                     alloc->meta[index].maxUnitLength);
            if(found == OBC_NULL_INDEX){
                return OBC_NULL_INDEX;
            }
        }

    }

    return freeLocation;

}

/**
OBC_ERROR_ENUM OBC_AllocatorExpand(void *allocator){

    OBC_Allocator *allocator_ = OBC_TO_RAW_ALLOCATOR(allocator);
    size_t i, index;

    if(OBC_AllocRayExpand(OBC_FROM_RAW_ALLOCRAY(& allocator_->rawAlloc))==OBC_ERROR_FAILURE){
        //no op re err
        return OBC_ERROR_FAILURE;
    }

    const unsigned int units = allocator_->metaUnits;
    size_t len;


    //switch(alloc->metaUnits){
    //case(0):
    //case(1):
    //case(2):
    //case(3):
    //case(4):
    //
    //}

    len = allocator_->rawAlloc.backed.curUnitLength;

    if(allocator_->metaUnits == 0){

        if(allocator_->rawAlloc.backed.curLength > OBC_ALLOC_UNITS_0){

            size_t start = allocator_->meta[0].maxLength;
            if(OBC_RayExpand(& allocator_->meta[0].rawData)== OBC_ERROR_FAILURE){
                return OBC_ERROR_FAILURE;
            }
            allocator_->metaUnits=1;
            memset(allocator_->meta[0].rawData+start,0,(allocator_->meta[0].maxLength)-start);

            return OBC_ERROR_SUCCESS;
        }

        return OBC_ERROR_NO_OP;
    }

    for(i = 0; i < units; i++){
        index = (units-i)-1;

        len = allocator_->meta[index].maxUnitLength
        const size_t metaMaxUnits =
            META_BITS*allocator_->meta[index].maxUnitLength;
        const size_t backedMaxUnits =
            allocator_->meta[index]

        if(OBC_RayExpand(& allocator_->meta[index].backed.rawData)
           ==OBC_ERROR_FAILURE){
            //shrink and release others

            index--;
            while(index){
                //
                //
                //
                index--;
            }

            return OBC_ERROR_FAILURE;
        }
        alloc->meta[index].rawData,
        alloc->meta[index].maxUnitLength);
        //
        if(found == OBC_NULL_INDEX){
            return OBC_NULL_INDEX;
        }
    }

    return OBC_ERROR_FAILURE;

}
//**/

/**

OBC_ERROR_ENUM OBC_AllocRayExpand(void *allocator){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    size_t i;

    if(OBC_RayDoExpand(& allocRay->backed.rawData) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    const size_t metaMaxUnits = BITS*allocRay->meta.maxUnitLength;
    const size_t backedMaxUnits = allocRay->backed.maxUnitLength;

    if(metaMaxUnits < backedMaxUnits){

        size_t start = allocRay->meta.maxLength;

        if(OBC_RayExpand(& allocRay->meta.rawData) == OBC_ERROR_FAILURE){
            for(i = 0; i < OBC_ALLOCATOR_MAX_CONTRACT_TRIES; i++){
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

//**/

/*
OBC_ERROR_ENUM OBC_AllocatorMarkAllocated(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;
    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((BITS-bits)-1);
    raw[unit] = raw[unit] | unitMask;

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_AllocatorMarkFreed(void *allocator, size_t pos){

    OBC_AllocRay *allocRay = OBC_TO_RAW_ALLOCRAY(allocator);
    OBC_ALLOCATOR_META_TYPE *raw = (OBC_ALLOCATOR_META_TYPE *)allocRay->meta.rawData;

    size_t unit = pos/BITS;
    size_t bits = (pos-(unit*BITS));
    OBC_ALLOCATOR_META_TYPE unitMask = 1<<((BITS-bits)-1);
    unitMask=~unitMask;

    raw[unit] = raw[unit] & unitMask;

    return OBC_ERROR_NO_OP;
}
*/

OBC_ERROR_ENUM OBC_AllocatorExpand(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;
    const size_t expansionLimits[OBC_ALLOCATOR_META_ADDRESSING] = {OBC_ALLOC_UNITS_0,OBC_ALLOC_UNITS_1,OBC_ALLOC_UNITS_2,OBC_ALLOC_UNITS_3};

    size_t i,j;

    if(units<4){
        if(allocator_->backed.curLength > expansionLimits[units]){
            allocator_->metaUnits = units+1;
        }
    }

    i = 0;
    do{

        if(OBC_RayExpand(& allocator_->meta[i].rawData)==OBC_ERROR_FAILURE){

            do{

                for(j = 0;
                    OBC_RayContract(& allocator_->meta[i-1].rawData)==OBC_ERROR_FAILURE
                    && j < OBC_ALLOCATOR_MAX_CONTRACT_TRIES; j++ ){
                    ///something horrible happened, continue to attempt other trimming
                }

                i--;
            }while(i);

            return OBC_ERROR_FAILURE;
        }

    }while(i < units);

    i=0;
    do{
        size_t start = allocator_->meta[i].maxLength/2;
        memset(allocator_->meta[i].rawData+start,0,((allocator_->meta[i].maxLength)-start));
        i++;
    }while(i < units);

    return OBC_ERROR_SUCCESS;

}

OBC_ERROR_ENUM OBC_AllocatorMarkAllocated(void *allocator){

    OBC_Allocator2 *allocator_ = OBC_TO_RAW_ALLOCATOR2(allocator);

    const unsigned int units = allocator_->metaUnits;

    size_t pos;
    size_t posAccum = 0;
    unsigned int i, index;
    for(i = 0; i < units; i++){
        index = (units-i)-1;

        pos = OBC_metaFirstEmptyBit((OBC_ALLOCATOR_META_TYPE *)allocator_->meta[index].rawData,allocator_->meta[index].maxUnitLength);
        if(pos==OBC_NULL_INDEX){
            return OBC_ERROR_FAILURE;
        }

        posAccum*=(OBC_ALLOCATOR_META_CHUNKSIZE*META_BITS);
        posAccum+=pos;
    }

    ///FIXME
    ///TODO heuristically apply used bits to meta lines

    return OBC_ERROR_SUCCESS;


}

OBC_ERROR_ENUM OBC_AllocatorMarkFreed(void *allocator){
    return OBC_ERROR_SUCCESS;
}
