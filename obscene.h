#ifndef OBSCENE_H_INCLUDED
#define OBSCENE_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

#define OBC_ENUM int

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

#include "obsceneqsort.h"
#include <stdlib.h>





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMPARE__SWITCH_SET_PTR(comparator, exitLabel, type, charStorageResult, ptr1, ptr2) \
switch( comparator ){\
    COMPARE__SWITCH_PTR( OBC_COMPARE_L_, exitLabel , type , <, charStorageResult, ptr1, ptr2);\
    COMPARE__SWITCH_PTR( OBC_COMPARE_G_, exitLabel , type ,> , charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_PTR( OBC_COMPARE_E_, exitLabel , type ,== , charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_PTR( OBC_COMPARE_LE_, exitLabel , type , <= ,charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_PTR( OBC_COMPARE_GE_, exitLabel , type , >=, charStorageResult, ptr1, ptr2);\
    COMPARE__SWITCH_PTR( OBC_COMPARE_NE_, exitLabel , type , !=, charStorageResult, ptr1, ptr2);\
    default:goto exitLabel;\
}
#define COMPARE__SWITCH_PTR(switch_value, exitLabel, type, action, charStorageResult, ptr1, ptr2) \
case(switch_value): \
    charStorageResult = ((void *) (ptr1) ) action ((void *) (ptr2) );\
    goto exitLabel;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMPARE__SWITCH_SET(comparator, exitLabel, type, charStorageResult, ptr1, ptr2) \
switch( comparator ){\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_L_, exitLabel , type , <, charStorageResult, ptr1, ptr2);\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_G_, exitLabel , type ,> , charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_E_, exitLabel , type ,== , charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_LE_, exitLabel , type , <= ,charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_GE_, exitLabel , type , >=, charStorageResult, ptr1, ptr2);\
    COMPARE__SWITCH_UNIT( OBC_COMPARE_NE_, exitLabel , type , !=, charStorageResult, ptr1, ptr2);\
    default:goto exitLabel;\
}
#define COMPARE__SWITCH_UNIT(switch_value, exitLabel, type, action, charStorageResult, ptr1, ptr2) \
case(switch_value): \
    charStorageResult = (*(type *) (ptr1) ) action (*(type *) (ptr2) );\
    goto exitLabel;
//*
#define COMPARE__DUMP_FULL_SWITCH(type, comparator, newExitLabelName, charStorageResult, ptr1, ptr2) \
do{\
switch(type){\
            case(OBC_ACTION_COMPARE_INT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, signed int, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_UINT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, unsigned int, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_FLOAT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, float, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_DOUBLE_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, double, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_CHAR_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, signed char, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_UCHAR_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, unsigned char, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_SHORT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, signed short, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_USHORT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, unsigned short, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_LLINT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, signed long long int, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_ULLINT_):\
                    COMPARE__SWITCH_SET(comparator, newExitLabelName, unsigned long long int, charStorageResult, ptr1, ptr2);\
            case(OBC_ACTION_COMPARE_CHARSTRING_):{break;}\
            case(OBC_ACTION_COMPARE_NODE_):\
                    COMPARE__SWITCH_SET_PTR(comparator, newExitLabelName, char*, charStorageResult, ptr1, ptr2);\
        }\
        newExitLabelName:;\
}while(0);




//https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
/* The PP_NARG macro returns the number of arguments that have been
  * passed to it.
  */

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define superTest(...) {printf("SIZE: %i\n",PP_NARG(__VA_ARGS__));linkedListTraversal_(__VA_ARGS__);}

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

void *OBC_LinkedListAppend(void *base, void *node, void *data);
void *OBC_LinkedListPrepend(void **base, void *node, void *data);
void *OBC_LinkedListInsert(void **base, void *node, void *cmprNode, void *data, OBC_ENUM type, OBC_ENUM compare);
void *OBC_LinkedListRemove(void **base, void *node, void *data);

void *OBC_traverseLinkedNodes(void *base, void *node);

//uses a char* as the base, as the size is the smallest primitive type to support as many systems as possible -- 8bit systems and up
//will throw a ptr casting warning in a compiler when the sizeof(char) is smaller than the sizeof(void *)
void* linkedListTraversal_(void *base, ...);

void *OBC_query (void *pbase, OBC_ENUM pbaseType, ...);


//#define OBC_ACTION_ ((void *)NULL)
enum OBC_QUERY{
    OBC_ACTION_ = (int)NULL,
    OBC_ACTION_END_,

    OBC_ACTION_DATANODE_,
    OBC_ACTION_NEXTNODE_,
    OBC_ACTION_NEXT_FUNC_ACCESOR_,

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

    //requires only THIS action's params
    OBC_ACTION_REVERSE_,
    OBC_ACTION_PREPEND_,
    OBC_ACTION_APPEND_,

    //comparator choices
    OBC_COMPARE_L_,
    OBC_COMPARE_G_,
    OBC_COMPARE_E_,
    OBC_COMPARE_LE_,
    OBC_COMPARE_GE_,
    OBC_COMPARE_NE_,

    //can have optional comparators to select which to cause
    OBC_ACTION_MIN_,
    OBC_ACTION_MAX_,
    OBC_ACTION_SUMMATION_,
    OBC_ACTION_AVERAGE_,
    OBC_ACTION_INSERT_,
    OBC_ACTION_REMOVE_,

    //specified query data grabbing actions
    OBC_QUERY_LINKED_LIST_,
    OBC_QUERY_LINKED_ARR_RAW_,
    OBC_QUERY_LINKED_ARR_PTR_,
    OBC_QUERY_RAW_ARRAY_,
    OBC_QUERY_PTR_ARRAY_,
    OBC_QUERY_HEAP_SPECIFIED_,

    OBC_TYPE_FLOAT,
    OBC_TYPE_DOUBLE,
    OBC_TYPE_CHAR,
    OBC_TYPE_UCHAR,
    OBC_TYPE_SHORT,
    OBC_TYPE_USHORT,
    OBC_TYPE_INT,
    OBC_TYPE_UINT,
    OBC_TYPE_LLINT,
    OBC_TYPE_ULLINT,
    OBC_TYPE_CHARSTRING

} OBC_QUERY;

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

///FIXME ADD FUNCTIONAL TO USE REQUIRED DATA -- FOR ARRAY
#define OBC_QUERY_LINKED_LIST(exitToken)
#define OBC_QUERY_PTR_ARRAY(length...)
#define OBC_QUERY_RAW_ARRAY(unitSize,length...)
#define OBC_CONST(size)


#define insertLinkedList(basePtr, nodePtr, dataBaseToAddPtr, args...) linkedListTraversal_(base, node, dataBaseToAddPtr, args, OBC_ACTION_END)

#endif // OBSCENE_H_INCLUDED
