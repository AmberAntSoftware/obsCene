#ifndef OBSCENE_H_INCLUDED
#define OBSCENE_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

#include "obsceneqsort.h"
#include <stdlib.h>

/*
void appendToLinkedStructure(void **base, void **baseNode);
void *removeFromLinkedStructure(void **base, void **baseNode, int nodeNumber);
void deleteFromLinkedStructure(void **base, void **baseNode, void (*freeFunction)(void *freeDataFunction));
void insertToLinkedStructure(void **base, void **baseNode, int nodeNumber);

void appendToLinkedStructure_(void *base, unsigned int nodeOffset);
void *removeFromLinkedStructure_(void **base, unsigned int nodeOffset, int nodeNumber);
void deleteFromLinkedStructure_(void *base, unsigned int nodeOffset, void (*freeFunction)(void *freeDataFunction));
void insertToLinkedStructure_(void *base, unsigned int nodeOffset, int nodeNumber);
*/

void *OBC_traverseLinkedNodes(void *base, void *node);

//uses a char* as the base, as the size is the smallest primitive type to support as many systems as possible -- 8bit systems and up
//will throw a ptr casting warning in a compiler when the sizeof(char) is smaller than the sizeof(void *)
void* linkedListTraversal_(void *base, ...);


#define OBC_ACTION_ ((void *)NULL)
enum OBC_QUERY{
    //OBC_ACTION_,
    OBC_ACTION_END_ = 1,

    OBC_ACTION_DATANODE_,
    OBC_ACTION_NEXTNODE_,

    OBC_ACTION_COMPARE_FLOAT_,
    OBC_ACTION_COMPARE_DOUBLE_,
    OBC_ACTION_COMPARE_CHAR_,
    OBC_ACTION_COMPARE_UCHAR_,
    OBC_ACTION_COMPARE_SHORT_,
    OBC_ACTION_COMPARE_USHORT_,
    OBC_ACTION_COMPARE_INT_,
    OBC_ACTION_COMPARE_UINT_,
    OBC_ACTION_COMPARE_LLINT_,
    OBC_ACTION_COMPARE_ULLINT_,
    OBC_ACTION_COMPARE_CHARSTRING_,
    OBC_ACTION_COMPARE_NODE_,

    OBC_ACTION_COMPARE_L_,
    OBC_ACTION_COMPARE_G_,
    OBC_ACTION_COMPARE_E_,
    OBC_ACTION_COMPARE_LE_,
    OBC_ACTION_COMPARE_GE_,
    OBC_ACTION_COMPARE_NE_,

    OBC_ACTION_INSERT_,
    OBC_ACTION_PREPEND_,
    OBC_ACTION_APPEND_,
    OBC_ACTION_MIN_,
    OBC_ACTION_MAX_,
    OBC_ACTION_REVERSE_,
    OBC_ACTION_SUMMATION_,
    OBC_ACTION_AVERAGE_,

    OBC_ACTION_LINKED_LIST_

} OBC_QUERY;

#define COMPARE_FLOAT OBC_ACTION_, OBC_ACTION_COMPARE_FLOAT_
#define COMPARE_DOUBLE OBC_ACTION_, OBC_ACTION_COMPARE_DOUBLE_
#define COMPARE_CHAR OBC_ACTION_, OBC_ACTION_COMPARE_CHAR_
#define COMPARE_UCHAR OBC_ACTION_, OBC_ACTION_COMPARE_UCHAR_
#define COMPARE_SHORT OBC_ACTION_, OBC_ACTION_COMPARE_SHORT_
#define COMPARE_USHORT OBC_ACTION_, OBC_ACTION_COMPARE_USHORT_
#define COMPARE_INT OBC_ACTION_, OBC_ACTION_COMPARE_INT_
#define COMPARE_UINT OBC_ACTION_, OBC_ACTION_COMPARE_UINT_
#define COMPARE_LLINT OBC_ACTION_, OBC_ACTION_COMPARE_LLINT_
#define COMPARE_ULLINT OBC_ACTION_, OBC_ACTION_COMPARE_ULLINT_
#define COMPARE_CHARS8 OBC_ACTION_, OBC_ACTION_COMPARE_CHARSTRING_, 8
#define COMPARE_CHARS16 OBC_ACTION_, OBC_ACTION_COMPARE_CHARSTRING_, 16
#define COMPARE_CHARS24 OBC_ACTION_, OBC_ACTION_COMPARE_CHARSTRING_, 24
#define COMPARE_CHARS32 OBC_ACTION_, OBC_ACTION_COMPARE_CHARSTRING_, 32

#define COMPARE_LESS_THAN OBC_ACTION_, OBC_ACTION_COMPARE_L_
#define COMPARE_GREATER_THAN OBC_ACTION_, OBC_ACTION_COMPARE_G_
#define COMPARE_EQUALS_TO OBC_ACTION_, OBC_ACTION_COMPARE_E_
#define COMPARE_LESS_THAN_OR_EQUALS_TO OBC_ACTION_, OBC_ACTION_COMPARE_LE_
#define COMPARE_GREATER_OR_EQUALS_TO OBC_ACTION_, OBC_ACTION_COMPARE_GE_

#define OBC_ACTION_INSERT OBC_ACTION_, OBC_ACTION_INSERT_
#define OBC_ACTION_PREPEND OBC_ACTION_, OBC_ACTION_PREPEND_
#define OBC_ACTION_APPEND OBC_ACTION_, OBC_ACTION_APPEND_
#define OBC_ACTION_MIN OBC_ACTION_, OBC_ACTION_MIN_
#define OBC_ACTION_MAX OBC_ACTION_, OBC_ACTION_MAX_
#define OBC_ACTION_REVERSE OBC_ACTION_, OBC_ACTION_REVERSE_
#define OBC_ACTION_SUMMATION OBC_ACTION_, OBC_ACTION_SUMMATION_
#define OBC_ACTION_AVERAGE OBC_ACTION_, OBC_ACTION_AVERAGE_

#define OBC_ACTION_END OBC_ACTION_, OBC_ACTION_END_
#define OBC_ACTION_NEXT_NODE(args...) OBC_ACTION_, OBC_ACTION_NEXTNODE_, args
#define OBC_ACTION_DATA_NODE(args...) OBC_ACTION_, OBC_ACTION_DATANODE_, args

#define insertLinkedList(basePtr, nodePtr, dataBaseToAddPtr, args...) linkedListTraversal_(base, node, dataBaseToAddPtr, args, OBC_ACTION_END)

#endif // OBSCENE_H_INCLUDED
