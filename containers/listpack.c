#include "list.h"

#define OBC_LIST_NEXT -1
#define OBC_LIST_PREV -2

void **OBC_newListPack(size_t unitSize){

    OBC_List *list = calloc(1,sizeof(OBC_List));
    if(OBC_initList(list,unitSize) == NULL){
        free(list);
        return NULL;
    }

    return (void **)OBC_FROM_ALLOCFASTBIT_VAL(list->allocator);

}
void *OBC_initListPack(OBC_List *list,size_t unitSize){

    if(OBC_initAllocFastBit(& list->allocator, unitSize+(sizeof(size_t)*2) ) == NULL){
        return NULL;
    }

    list->first = OBC_NULL_INDEX;
    list->last = OBC_NULL_INDEX;

    return list;
}

void OBC_freeListPack(void **raw){

    OBC_List *list = OBC_TO_LIST_PTR(raw);
    OBC_freeListData(list);
    free(list);

}
void OBC_freeListPackData(OBC_List *list){

    OBC_freeAllocFastBit(OBC_FROM_ALLOCFASTBIT_VAL(list->allocator));

}

OBC_Offset OBC_ListPackNewItem(OBC_List *list){

    size_t place = OBC_AllocFastBitMalloc(OBC_FROM_ALLOCFASTBIT_VAL(list->allocator));
    if(place == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }
    size_t *data;
    if(list->first == OBC_NULL_INDEX){
        list->first = place;
        list->last = place;

        /*
        data = (size_t *)(list->allocator.backed.rawData+
                          (list->allocator.backed.unitSize*(place+1)));
        data[OBC_LIST_NEXT] = OBC_NULL_INDEX;
        data[OBC_LIST_PREV] = OBC_NULL_INDEX;
        return place;
        //*/
    }

    data = (size_t *)(list->allocator.backed.rawData+
                              (list->allocator.backed.unitSize*(list->last+1)));
    data[OBC_LIST_NEXT] = place;

    data = (size_t *)(list->allocator.backed.rawData+
                              (list->allocator.backed.unitSize*(place+1)));
    data[OBC_LIST_NEXT] = OBC_NULL_INDEX;
    data[OBC_LIST_PREV] = list->last;
    list->last = place;

    return place;
}
OBC_Offset OBC_ListPackNewItemRaw(void **raw){
    return OBC_ListPackNewItem(OBC_TO_LIST_PTR(raw));
}

OBC_ERROR_ENUM OBC_ListPackAdd(OBC_List *list, void *item){
    //
    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_ListPackAddRaw(void **raw, void *item){
    //
    return OBC_ERROR_SUCCESS;
}

OBC_ERROR_ENUM OBC_ListPackRemove(OBC_List *list, OBC_Offset freeItem){
    //
    return OBC_ERROR_SUCCESS;
}
OBC_ERROR_ENUM OBC_ListPackRemoveRaw(void **raw, OBC_Offset freeItem){
    //
    return OBC_ERROR_SUCCESS;
}

OBC_Offset OBC_ListPackIterStart(OBC_List *list){
    if(list->first == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }else{
        return list->first;
    }
}

OBC_Offset OBC_ListPackIterStartRaw(void **raw){
    return OBC_ListIterStartRaw(OBC_TO_LIST_PTR(raw));
}

OBC_Offset OBC_ListPackIterNext(OBC_List *list, const OBC_Offset iter){
    if(iter == OBC_NULL_INDEX){
        return OBC_NULL_INDEX;
    }
    /*const size_t where = (iter+1) * (list->allocator.backed.unitSize);
    const size_t * const next = (const size_t * const)(list->allocator.backed.rawData+where);*/
    size_t * next = (size_t *)(list->allocator.backed.rawData+
                              (list->allocator.backed.unitSize*(iter+1)));

    printf("%u\n",next[OBC_LIST_PREV]);
    printf("%u\n",next[OBC_LIST_NEXT]);
    const size_t found = next[OBC_LIST_NEXT];

    return found;
}

OBC_Offset OBC_ListPackIterNextRaw(void **raw, const OBC_Offset iter){
    return OBC_ListPackIterNext(OBC_TO_LIST_PTR(raw), iter);
}
