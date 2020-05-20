#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "../obc.h"
#include "../allocators/alloclistbit.h"

typedef struct OBC_Tree{

    OBC_AllocListBit allocator;
    OBC_Ray links;
    OBC_Offset root;

    size_t count;

}OBC_Tree;

typedef struct OBC_TreeUnit{

    OBC_Offset value;
    OBC_Offset parent;

}OBC_TreeUnit;

/*************************************
Initialization / Deallocation
*************************************/

void **OBC_newTree(size_t unitSize);
void *OBC_initTree(OBC_Tree *tree, size_t unitSize);
void **OBC_TreeGetDataPointer(OBC_Tree *tree);

void OBC_freeTree(void *arr);
void OBC_freeTreeData(OBC_Tree *tree);


/*************************************
Iteration
*************************************/

typedef struct OBC_TreeIterator{

    OBC_Offset next;
    OBC_Offset prev;

    unsigned char didLeft;
    unsigned char didRight;

}OBC_TreeIterator;


#define OBC_TreeForEach(arrPtr, iter) for(iter = OBC_TreeIterStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNext(arrPtr, iter))
#define OBC_TreeForEachRaw(listPtr, iter) for(iter = OBC_TreeIterStartRaw(listPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNextRaw(listPtr, iter))

OBC_Offset OBC_TreeIterStartRaw(OBC_Tree *tree);
OBC_Offset OBC_TreeIterStart(void *arr);

OBC_Offset OBC_TreeIterNextRaw(OBC_Tree *tree, OBC_TreeIterator* iter);
OBC_Offset OBC_TreeIterNext(void *arr, OBC_TreeIterator* iter);

#define OBC_TreeAddLoop(arrPtr, itemPtr, iter, spaceshipValue) for(iter = OBC_TreeAddStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeAddNext(arrPtr, itemPtr, iter, spaceshipValue))
#define OBC_TreeAddLoopRaw(treePtr, itemPtr, iter, spaceshipValue) for(iter = OBC_TreeAddStartRaw(treePtr); iter != OBC_NULL_INDEX; iter = OBC_TreeAddNextRaw(treePtr, itemPtr, iter, spaceshipValue))

OBC_Offset OBC_TreeAddStartRaw(OBC_Tree *tree);
OBC_Offset OBC_TreeAddStart(void *arr);

OBC_Offset OBC_TreeAddNextRaw(OBC_Tree *tree, void *itemToAdd, OBC_Offset iter, int spaceshipValue);
OBC_Offset OBC_TreeAddNext(void *arr, void *itemToAdd, OBC_Offset iter, int spaceshipValue);



/*************************************
Internal Utilities
*************************************/

#define _OBC_TREE_PTR_CAST(rawPtr) ((OBC_Tree *)(rawPtr))
#define _OBC_TREE_OFFSET ((size_t)(&((OBC_Tree *)NULL)->allocator.backed.rawData))
#define OBC_TO_TREE_PTR(rawPtr) (_OBC_TREE_PTR_CAST(((void*)(rawPtr)) - _OBC_TREE_OFFSET))

#define OBC_FROM_TREE_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_TREE_OFFSET))
#define OBC_FROM_TREE_VAL(treeVal) ((void**)(((void*)(&(treeVal))) + _OBC_TREE_OFFSET))



OBC_Offset OBC_TreeLinearIterStartRaw(OBC_Tree *tree);
OBC_Offset OBC_TreeLinearIterStart(void *arr);

OBC_Offset OBC_TreeLinearIterNextRaw(OBC_Tree *tree, OBC_Offset iter);
OBC_Offset OBC_TreeLinearIterNext(void *arr, OBC_Offset iter);


#endif // TREE_H_INCLUDED
