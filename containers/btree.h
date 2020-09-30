#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "../obc.h"
#include "../allocators/alloclistbit.h"

typedef struct OBC_BTree{

    OBC_AllocListBit allocator;
    OBC_Ray links;

    OBC_Offset root;
    size_t count;

}OBC_BTree;

/*************************************
Initialization
*************************************/

void **OBC_newBTree(size_t unitSize);
OBC_ERROR_ENUM OBC_initBTree(OBC_BTree *tree, size_t unitSize);

/*************************************
Deallocation
*************************************/

void OBC_freeBTree(void *arr);
void OBC_freeBTreeData(OBC_BTree *tree);

/*************************************
Data Accessors
*************************************/

void **OBC_BTreeGetDataPointer(OBC_BTree *tree);

/*************************************
Operations
*************************************/

OBC_ERROR_ENUM OBC_BTreeDelete(void *arr, OBC_Offset index);
OBC_ERROR_ENUM OBC_BTreeDeleteRaw(OBC_BTree *tree, OBC_Offset index);

OBC_ERROR_ENUM OBC_BTreeDeleteNode(void *arr, OBC_Offset index);
OBC_ERROR_ENUM OBC_BTreeDeleteNodeRaw(OBC_BTree *tree, OBC_Offset index);

OBC_Offset OBC_BTreeNewNode(void *arr);
OBC_Offset OBC_BTreeNewNodeRaw(OBC_BTree *tree);

#define OBC_BTreePutNodeLoop(arrPtr,btreeIteratorPtr)

/*************************************
Iteration
*************************************/

typedef struct OBC_BTreeIterator{

    OBC_Offset iter;

    OBC_SpaceshipValue cmpr;

    //OBC_Dequeue traversal;

}OBC_BTreeIterator;


#define OBC_BTreeForEach(arrPtr, listIterPtr) for(OBC_TreeIterStart(arrPtr, listIterPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNext(arrPtr, iter))
#define OBC_BTreeForEachRaw(listPtr, listIterPtr) for(OBC_TreeIterStartRaw(listPtr, listIterPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeIterNextRaw(listPtr, iter))

OBC_Offset OBC_TreeIterStartRaw(OBC_BTree *tree);
OBC_Offset OBC_TreeIterStart(void *arr);

OBC_Offset OBC_TreeIterNextRaw(OBC_BTree *tree, OBC_BTreeIterator* iter);
OBC_Offset OBC_TreeIterNext(void *arr, OBC_BTreeIterator* iter);

#define OBC_TreeAddLoop(arrPtr, itemPtr, iter, spaceshipValue) for(iter = OBC_TreeAddStart(arrPtr); iter != OBC_NULL_INDEX; iter = OBC_TreeAddNext(arrPtr, itemPtr, iter, spaceshipValue))
#define OBC_TreeAddLoopRaw(treePtr, itemPtr, iter, spaceshipValue) for(iter = OBC_TreeAddStartRaw(treePtr); iter != OBC_NULL_INDEX; iter = OBC_TreeAddNextRaw(treePtr, itemPtr, iter, spaceshipValue))

OBC_Offset OBC_TreeAddStartRaw(OBC_BTree *tree);
OBC_Offset OBC_TreeAddStart(void *arr);

OBC_Offset OBC_TreeAddNextRaw(OBC_BTree *tree, void *itemToAdd, OBC_Offset iter, int spaceshipValue);
OBC_Offset OBC_TreeAddNext(void *arr, void *itemToAdd, OBC_Offset iter, int spaceshipValue);



/*************************************
Internal Utilities
*************************************/

#define _OBC_BTREE_PTR_CAST(rawPtr) ((OBC_BTree *)(rawPtr))
#define _OBC_BTREE_OFFSET ((size_t)(&((OBC_BTree *)NULL)->allocator.backed.rawData))
#define OBC_TO_BTREE_PTR(rawPtr) (_OBC_BTREE_PTR_CAST(((void*)(rawPtr)) - _OBC_BTREE_OFFSET))

#define OBC_FROM_BTREE_PTR(rawPtr) ((void**)(((void*)(rawPtr)) + _OBC_BTREE_OFFSET))
#define OBC_FROM_BTREE_VAL(treeVal) ((void**)(((void*)(&(treeVal))) + _OBC_BTREE_OFFSET))

typedef struct OBC_BTreeNode{

    //mirrors allocators so not needed
    //OBC_Offset value;

    OBC_Offset left;
    OBC_Offset right;

}OBC_BTreeNode;


OBC_Offset OBC_TreeLinearIterStartRaw(OBC_BTree *tree);
OBC_Offset OBC_TreeLinearIterStart(void *arr);

OBC_Offset OBC_TreeLinearIterNextRaw(OBC_BTree *tree, OBC_Offset iter);
OBC_Offset OBC_TreeLinearIterNext(void *arr, OBC_Offset iter);


#endif // TREE_H_INCLUDED
