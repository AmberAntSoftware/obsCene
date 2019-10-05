#ifndef OBSCENE_H_INCLUDED
#define OBSCENE_H_INCLUDED

#include "obsceneinternals.h"
#include "obsceneqsort.h"


//#define SAFE_OBC 1

/*
struct kl{
int a;
int b;
}kl;

void func(){

kl a;
kl b;
kl c;
kl *things[3] = {&a,&b,&c};

int a = (int)OBC_query(OBC_TYPE_PRT_ARR,things,OBC_ACTION_MAX(OBC_MAX_INT,a.a));

}
*/


/*/
#define COMPARE__DUMP_FULL_SWITCH(type, comparator, newExitLabelName, charStorageResult, ptr1, ptr2) \
do{\
charStorageResult = *(unsigned long long int*)ptr1 < *(unsigned long long int*)ptr2;\
}while(0);
//*/

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

/*
Individual basic functions
*/

///FIXME ADD FUNCTIONAL TO USE REQUIRED DATA -- FOR ARRAY
#define OBC_QUERY_LINKED_LIST(exitToken)
#define OBC_QUERY_PTR_ARRAY(length...)
#define OBC_QUERY_RAW_ARRAY(unitSize,length...)

#define OBC_CONST(size) OBC_VALUE_CONST_, size

#define OBC_QUERY(OBC_ACTION, OBC_NODEPATH, ...) /*OBC_QUERY(*/ PP_NARG(__VA_ARGS__)+3, OBC_NODEPATH, __VA_ARGS__

void *OBC_LinkedListAppend(void *base, void *node, void *data);
void *OBC_LinkedListPrepend(void **base, void *node, void *data);
void *OBC_LinkedListInsert(void **base, void *node, void *cmprNode, void *data, OBC_ENUM type, OBC_ENUM compare);
void *OBC_LinkedListRemove(void **base, void *node, void *data);

void *OBC_ArrayPrepend(void *base, void *data, size_t unitSize, size_t arrLen);
void *OBC_ArrayInsert(void *base, void *cmprNode, void *data, size_t unitSize, size_t arrLen, OBC_ENUM type, OBC_ENUM compare);


void *OBC_traverseLinkedNodes(void *base, void *node);

//uses a char* as the base, as the size is the smallest primitive type to support as many systems as possible -- 8bit systems and up
//will throw a ptr casting warning in a compiler when the sizeof(char) is smaller than the sizeof(void *)
void* linkedListTraversal_(void *base, ...);

void *OBC_query (void *pbase, OBC_ENUM pbaseType, ...);

//#define OBC_ACTION_ ((void *)NULL)

///FIXME ADD FUNCTIONAL TO INLINE DATA PATH -- PREFIX NOTATION
///E.G: (COMPARATOR, AT_LEAST_ONE_OFFSET_DATA, ...more)
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
