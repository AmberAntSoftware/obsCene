#include "tree.h"

#define OBC_TREE_LINK_COUNT 2
#define OBC_TREE_LEFT 0
#define OBC_TREE_RIGHT 1

/**
void **OBC_newTree(size_t unitSize){

    OBC_Tree *tree = calloc(1,sizeof(OBC_Tree));

    if(OBC_initTree(tree,unitSize) == NULL){
        free(tree);
        return NULL;
    }

    return (void **)OBC_FROM_ALLOCFASTBIT_VAL(tree->allocator);

}
void *OBC_initTree(OBC_Tree *tree,size_t unitSize){

    if(OBC_initRay(& tree->links,0,sizeof(size_t)*OBC_TREE_LINK_COUNT) == NULL){
        return NULL;
    }

    if(OBC_initAllocFastBit(& tree->allocator, unitSize ) == NULL){
        OBC_freeRayData(& tree->links);
        return NULL;
    }

    tree->root = OBC_NULL_INDEX;

    return tree;
}
void **OBC_TreeGetAccessPointer(OBC_Tree *tree){
    return (void **)OBC_FROM_ALLOCFASTBIT_VAL(tree->allocator);
}

void OBC_freeTree(void *arr){

    OBC_Tree *tree = OBC_TO_TREE_PTR(arr);
    OBC_freeTreeData(tree);
    free(tree);

}
void OBC_freeTreeData(OBC_Tree *tree){

    OBC_freeAllocFastBit(OBC_FROM_ALLOCFASTBIT_VAL(tree->allocator));
    OBC_freeRay(OBC_FROM_RAY_VAL(tree->links));

}




OBC_ERROR_ENUM OBC_TreeRemoveRaw(OBC_Tree *tree, OBC_Offset parentItem, OBC_Offset freeItem){

    size_t *data = ((size_t *)(tree->links.rawData))+freeItem*OBC_TREE_LINK_COUNT;

    if(OBC_AllocFastBitFree(OBC_FROM_ALLOCFASTBIT_VAL(tree->allocator),freeItem) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    if(data[OBC_LIST_PREV] == freeItem){
        tree->first = data[OBC_LIST_NEXT];
        if(data[OBC_LIST_NEXT] == OBC_NULL_INDEX){
            tree->last = OBC_NULL_INDEX;
        }
        return OBC_ERROR_SUCCESS;
    }

    size_t next = data[OBC_LIST_NEXT];

    data = ((size_t *)(tree->links.rawData))+data[OBC_LIST_PREV]*OBC_TREE_LINK_COUNT;
    data[OBC_LIST_NEXT] = next;

    return OBC_ERROR_SUCCESS;
}






OBC_Offset OBC_TreeLinearIterStartRaw(OBC_Tree *tree){
    return 0;
}
OBC_Offset OBC_TreeLinearIterStart(void *arr){
    return OBC_TreeLinearIterStartRaw(OBC_TO_TREE_PTR(arr));
}

OBC_Offset OBC_TreeLinearIterNextRaw(OBC_Tree *tree, OBC_Offset iter){
    if(iter >= tree->allocator.backed.curUnitLength){
        return OBC_NULL_INDEX;
    }
    const unsigned char *const raw = (const unsigned char *const)tree->allocator.meta.rawData;

    char found;
    do{
        iter+=1;
    }while(
      ((raw[(iter)/sizeof(unsigned char)]
          >>((CHAR_BIT-1)-(iter-((iter/sizeof(unsigned char))*sizeof(unsigned char))))
          )&0x1) == 0);
    return iter;
}
OBC_Offset OBC_TreeLinearIterNext(void *arr, OBC_Offset iter){
    return OBC_TreeLinearIterNextRaw(OBC_TO_TREE_PTR(arr),iter);
}



OBC_Offset OBC_TreeAddStartRaw(OBC_Tree *tree){
    return tree->root;
}
OBC_Offset OBC_TreeAddStart(void *arr){
    return OBC_TreeAddStartRaw(OBC_TO_TREE_PTR(arr));
}

OBC_Offset OBC_TreeAddNextRaw(OBC_Tree *tree, void *itemToAdd, OBC_Offset iter, int spaceshipValue){

    const size_t * const data = ((size_t *)(tree->links.rawData))+iter*OBC_TREE_LINK_COUNT;

}
OBC_Offset OBC_TreeAddNext(void *arr, void *itemToAdd, OBC_Offset iter, int spaceshipValue){
    return OBC_TreeAddNextRaw(OBC_TO_TREE_PTR(arr),itemToAdd,iter, spaceshipValue);
}









OBC_Offset OBC_TreeIterStartRaw(OBC_Tree *tree){
    return tree->root;
}
OBC_Offset OBC_TreeIterStart(void *arr){
    return OBC_TreeIterStartRaw(OBC_TO_TREE_PTR(arr));
}

OBC_Offset OBC_TreeIterNextRaw(OBC_Tree *tree, OBC_Offset iter){
    //
}
OBC_Offset OBC_TreeIterNext(void *arr, OBC_Offset iter){
    return OBC_TreeIterNextRaw(OBC_TO_TREE_PTR(arr),iter);
}

















OBC_ERROR_ENUM OBC_TreeDelete(OBC_Tree *tree, void *itemToAdd, OBC_Offset parentOfParent, OBC_Offset parent, OBC_Offset child){

    const size_t * const data = ((size_t *)(tree->links.rawData))+iter*OBC_TREE_LINK_COUNT;

    size_t * const parent_ = ((size_t *)(tree->links.rawData))+parent*OBC_TREE_LINK_COUNT;
    size_t * const child_ = ((size_t *)(tree->links.rawData))+child*OBC_TREE_LINK_COUNT;

    if(child_[OBC_TREE_LEFT] == OBC_NULL_INDEX && child_[OBC_TREE_RIGHT] == OBC_NULL_INDEX){
        ///SET PARENT TO NULL
        return OBC_ERROR_SUCCESS;
    }

    if(child_[OBC_TREE_LEFT] != OBC_NULL_INDEX && child_[OBC_TREE_RIGHT] != OBC_NULL_INDEX){
        ///GET LEST IN INORDER TRAVERSAL -- on right
        return OBC_ERROR_SUCCESS;
    }s



    return OBC_ERROR_SUCCESS;
}
//*/
