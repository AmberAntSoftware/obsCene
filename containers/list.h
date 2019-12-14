#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "OBC.h"
#include "allocfastbit.h"

typedef struct BadList{

    int value;

    struct BadList *next;

}BadList;

#define _OBC_LIST_PTR_CAST(rawPtr) ((OBC_List *)(rawPtr))
#define _OBC_LIST_OFFSET ((size_t)(&((OBC_List *)NULL)->allocator.backed.rawData))
#define OBC_TO_RAW_LIST(rawPtr) (_OBC_LIST_PTR_CAST(((void*)(rawPtr)) - _OBC_LIST_OFFSET))

///Note: internal order is guided by data at the end of the supplied data size
typedef struct OBC_List{

    OBC_AllocFastBit allocator;
    OBC_Ray links;
    OBC_Offset last;
    OBC_Offset first;

}OBC_List;

void **OBC_newList(size_t elementSize);
void *OBC_initList(OBC_List *list,size_t unitSize);

void OBC_freeList(void **raw);
void OBC_freeListData(OBC_List *list);

OBC_Offset OBC_ListNewItem(OBC_List *list);
OBC_Offset OBC_ListNewItemRaw(void **raw);

OBC_ERROR_ENUM OBC_ListInsert(OBC_List *list, size_t item, size_t insertLocation);
OBC_ERROR_ENUM OBC_ListInsertRaw(void **raw, size_t item, size_t insertLocation);

OBC_ERROR_ENUM OBC_ListRemove(OBC_List *list, size_t freeItem);
OBC_ERROR_ENUM OBC_ListRemoveRaw(void **raw, size_t freeItem);

OBC_ERROR_ENUM OBC_ListAdd(OBC_List *list, void *item);
OBC_ERROR_ENUM OBC_ListAddRaw(void **raw, void *item);

OBC_ERROR_ENUM OBC_ListRemove(OBC_List *list, size_t freeItem);
OBC_ERROR_ENUM OBC_ListRemoveRaw(void **raw, size_t freeItem);

OBC_Offset OBC_ListIterStart(OBC_List *list);
OBC_Offset OBC_ListIterStartRaw(void **raw);

OBC_Offset OBC_ListIterNext(OBC_List *list, const OBC_Offset iter);
OBC_Offset OBC_ListIterNextRaw(void **raw, const OBC_Offset iter);

#define OBC_GetPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_GetValue(rawPtr, index) ((*(rawPtr))[index])


#endif // LIST_H_INCLUDED
