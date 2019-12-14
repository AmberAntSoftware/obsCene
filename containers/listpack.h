#ifndef LISTPACK_H_INCLUDED
#define LISTPACK_H_INCLUDED


#include "OBC.h"
#include "allocfastbit.h"

typedef struct BadList{

    int value;

    struct BadList *next;

}BadList;

#define _OBC_LISTPACK_PTR_CAST(rawPtr) ((OBC_ListPack *)(rawPtr))
#define _OBC_LISTPACK_OFFSET ((size_t)(&((OBC_ListPack *)NULL)->allocator.backed.rawData))
#define OBC_TO_RAW_LISTPACK(rawPtr) (_OBC_LISTPACK_PTR_CAST(((void*)(rawPtr)) - _OBC_LISTPACK_OFFSET))

///Note: internal order is guided by data at the end of the supplied data size
typedef struct OBC_ListPack{

    OBC_AllocFastBit allocator;
    OBC_Offset last;
    OBC_Offset first;

}OBC_ListPack;

void *OBC_newListPack(size_t elementSize);
void *OBC_initListPack(OBC_ListPack *list,size_t unitSize);

void OBC_freeListPack(void **raw);
void OBC_freeListPackData(OBC_ListPack *list);

OBC_Offset OBC_ListPackNewItem(OBC_ListPack *list);
OBC_Offset OBC_ListPackNewItemRaw(void **raw);

OBC_ERROR_ENUM OBC_ListPackInsert(OBC_ListPack *list, size_t item, size_t insertLocation);
OBC_ERROR_ENUM OBC_ListPackInsertRaw(void **raw, size_t item, size_t insertLocation);

OBC_ERROR_ENUM OBC_ListPackRemove(OBC_ListPack *list, size_t freeItem);
OBC_ERROR_ENUM OBC_ListPackRemoveRaw(void **raw, size_t freeItem);

OBC_ERROR_ENUM OBC_ListPackAdd(OBC_ListPack *list, void *item);
OBC_ERROR_ENUM OBC_ListPackAddRaw(void **raw, void *item);

OBC_ERROR_ENUM OBC_ListPackRemove(OBC_ListPack *list, size_t freeItem);
OBC_ERROR_ENUM OBC_ListPackRemoveRaw(void **raw, size_t freeItem);

OBC_Offset OBC_ListIterPackStart(OBC_ListPack *list);
OBC_Offset OBC_ListIterPackStartRaw(void **raw);

OBC_Offset OBC_ListIterPackNext(OBC_ListPack *list, const OBC_Offset iter);
OBC_Offset OBC_ListIterPackNextRaw(void **raw, const OBC_Offset iter);

#define OBC_GetListPackPointer(rawPtr, index) ((*(rawPtr))+index)
#define OBC_GetListPackValue(rawPtr, index) ((*(rawPtr))[index])

#endif // LISTPACK_H_INCLUDED
