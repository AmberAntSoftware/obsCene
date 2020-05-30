#include "list.h"

#define OBC_LIST_LINK_COUNT 2
#define OBC_LIST_NEXT 0
#define OBC_LIST_PREV 1

void **OBC_newList(size_t unitSize){

    OBC_List *list = calloc(1,sizeof(OBC_List));

    if(list == NULL){
        return NULL;
    }

    if(OBC_initList(list, unitSize) == OBC_ERROR_FAILURE){
        free(list);
        return NULL;
    }

    return OBC_ListGetDataPointer(list);

}
OBC_ERROR_ENUM OBC_initList(OBC_List *list, size_t unitSize){

    if(OBC_initRayComplex(& list->links, 0, sizeof(size_t)*OBC_LIST_LINK_COUNT) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

#ifdef OBC_LIST_FAST
    if(OBC_initAllocList(& list->allocator, unitSize) == OBC_ERROR_FAILURE){
        OBC_freeRayData(& list->links);
        return OBC_ERROR_FAILURE;
    }
#else
    if(OBC_initAllocListBit(& list->allocator, unitSize) == OBC_ERROR_FAILURE){
        OBC_freeRayData(& list->links);
        return OBC_ERROR_FAILURE;
    }
#endif


    list->first = OBC_NULL_INDEX;
    list->last = OBC_NULL_INDEX;

    return OBC_ERROR_SUCCESS;
}

void **OBC_ListGetDataPointer(OBC_List *list){
    return OBC_AllocListBitGetDataPointer(&list->allocator);
}

void OBC_freeList(void *raw){

    OBC_List *list = OBC_TO_LIST_PTR(raw);
    OBC_freeListData(list);
    free(list);

}
void OBC_freeListData(OBC_List *list){

    OBC_freeAllocListBitData(& list->allocator);
    OBC_freeRayData(& list->links);

}

OBC_Offset OBC_ListNewItemRaw(OBC_List *list){

#ifdef OBC_LIST_FAST
    size_t place = OBC_AllocListMallocRaw(& list->allocator);
#else
    size_t place = OBC_AllocListBitMallocRaw(& list->allocator);
#endif

    if(place == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }
    if(list->links.maxUnitLength <= place){
        ///consider no_op error as well due to unsync ray alignments
        if(OBC_RayExpand(OBC_FROM_RAY_VAL(list->links)) != OBC_ERROR_SUCCESS){
            return OBC_NULL_INDEX;
        }
    }

    size_t *data;
    if(list->first == OBC_NULL_INDEX){
        list->first = place;
        list->last = place;
    }

    data = ((size_t *)(list->links.rawData))+list->last*OBC_LIST_LINK_COUNT;
    data[OBC_LIST_NEXT] = place;

    data = ((size_t *)(list->links.rawData))+place*OBC_LIST_LINK_COUNT;
    data[OBC_LIST_NEXT] = OBC_NULL_INDEX;
    data[OBC_LIST_PREV] = list->last;
    list->last = place;

    return place;
}
OBC_Offset OBC_ListNewItem(void *raw){
    return OBC_ListNewItemRaw(OBC_TO_LIST_PTR(raw));
}


OBC_ERROR_ENUM OBC_ListAdd(void *raw, void *item){

    return OBC_ListAddRaw(OBC_TO_LIST_PTR(raw),item);
}

OBC_ERROR_ENUM OBC_ListAddRaw(OBC_List *list, void *item){

    const size_t pos = OBC_ListNewItemRaw(list);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    OBC_X_MEMCPY(list->allocator.backed.rawData,list->allocator.backed.unitSize,pos,item);


    return OBC_ERROR_SUCCESS;
}



OBC_ERROR_ENUM OBC_ListRemoveRaw(OBC_List *list, OBC_Offset freeItem){

    size_t *data = ((size_t *)(list->links.rawData))+freeItem*OBC_LIST_LINK_COUNT;

#ifdef OBC_LIST_FAST
    if(OBC_AllocListFreeRaw(& list->allocator,freeItem) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
#else
    if(OBC_AllocListBitFreeRaw(& list->allocator,freeItem) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }
#endif


    if(data[OBC_LIST_PREV] == freeItem){
        list->first = data[OBC_LIST_NEXT];
        if(data[OBC_LIST_NEXT] == OBC_NULL_INDEX){
            list->last = OBC_NULL_INDEX;
        }
        return OBC_ERROR_SUCCESS;
    }

    size_t next = data[OBC_LIST_NEXT];

    data = ((size_t *)(list->links.rawData))+data[OBC_LIST_PREV]*OBC_LIST_LINK_COUNT;
    data[OBC_LIST_NEXT] = next;

    return OBC_ERROR_SUCCESS;
}
OBC_ERROR_ENUM OBC_ListRemove(void *raw, OBC_Offset freeItem){
    return OBC_ListRemoveRaw(OBC_TO_LIST_PTR(raw),freeItem);
}

OBC_Offset OBC_ListIterStartRaw(OBC_List *list){
    if(list->first == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }else{
        return list->first;
    }
}

OBC_Offset OBC_ListIterStart(void *raw){
    return OBC_ListIterStartRaw(OBC_TO_LIST_PTR(raw));
}

OBC_Offset OBC_ListIterNextRaw(OBC_List *list, const OBC_Offset iter){
    if(iter == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }

    const size_t * next = ((size_t *)(list->links.rawData))+(iter*OBC_LIST_LINK_COUNT);
    const size_t found = next[OBC_LIST_NEXT];

    return found;
}

OBC_Offset OBC_ListIterNext(void *raw, const OBC_Offset iter){
    return OBC_ListIterNextRaw(OBC_TO_LIST_PTR(raw), iter);
}
