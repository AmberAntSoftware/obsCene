#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "../obc.h"
#include "../allocators/alloclistbit.h"

typedef struct OBC_List{

    OBC_AllocListBit allocator;
    OBC_Ray links;
    OBC_Offset last;
    OBC_Offset first;

    size_t count;

}OBC_List;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newList(size_t unitSize);
//void **OBC_newListComplex(size_t unitSize, OBC_LIST_ALLOC allocChoice);
OBC_ERROR_ENUM OBC_initList(OBC_List *list, size_t unitSize);
//OBC_ERROR_ENUM OBC_initListComplex(OBC_List *list, size_t unitSize, OBC_LIST_ALLOC allocChoice);

void **OBC_ListGetDataPointer(OBC_List *list);

void OBC_freeList(void *arr);
void OBC_freeListData(OBC_List *list);

OBC_Offset OBC_ListNewItemRaw(OBC_List *list);
OBC_Offset OBC_ListNewItem(void *arr);

///TODO IMPLEMENT change header to fit with ListAdd
OBC_ERROR_ENUM OBC_ListInsertRaw(OBC_List *list, OBC_Offset item, OBC_Offset insertLocation);
OBC_ERROR_ENUM OBC_ListInsert(void *arr, OBC_Offset item, OBC_Offset insertLocation);

OBC_ERROR_ENUM OBC_ListRemoveRaw(OBC_List *list, OBC_Offset freeItem);
OBC_ERROR_ENUM OBC_ListRemove(void *arr, OBC_Offset freeItem);

OBC_ERROR_ENUM OBC_ListAddRaw(OBC_List *list, void *item);
OBC_ERROR_ENUM OBC_ListAdd(void *arr, void *item);

/*************************************
Iteration
*************************************/

typedef OBC_Offset OBC_ListIterator;
#define OBC_ListForEach(arrPtr, iter) for(iter = OBC_ListIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_ListIterNext(arrPtr, iter))
#define OBC_ListForEachRaw(listPtr, iter) for(iter = OBC_ListIterStartRaw(listPtr); iter != OBC_NULL_INDEX; iter = OBC_ListIterNextRaw(listPtr, iter))


/*************************************
Alternate Utilities
*************************************/

#define OBC_ListAddRawFast(arrPtr, itemValue)\
{\
if(OBC_ListNewItem(arrPtr) != OBC_NULL_INDEX){\
   ((*(arrPtr))[OBC_TO_LIST_PTR(arrPtr)->last]) = itemValue;\
}\
}


/*************************************
Internal Utilities
*************************************/

#define _OBC_LIST_PTR_CAST(arrPtr) ((OBC_List *)(arrPtr))
#define _OBC_LIST_OFFSET ((size_t)(&((OBC_List *)NULL)->allocator.backed.rawData))
#define OBC_TO_LIST_PTR(arrPtr) (_OBC_LIST_PTR_CAST(((void*)(arrPtr)) - _OBC_LIST_OFFSET))

#define OBC_FROM_LIST_PTR(arrPtr) ((void**)(((void*)(arrPtr)) + _OBC_LIST_OFFSET))
#define OBC_FROM_LIST_VAL(listVal) ((void**)(((void*)(&(listVal))) + _OBC_LIST_OFFSET))



OBC_Offset OBC_ListIterStartRaw(OBC_List *list);
OBC_Offset OBC_ListIterStart(void *arr);

OBC_Offset OBC_ListIterNextRaw(OBC_List *list, const OBC_Offset iter);
OBC_Offset OBC_ListIterNext(void *arr, const OBC_Offset iter);



#endif // LIST_H_INCLUDED
