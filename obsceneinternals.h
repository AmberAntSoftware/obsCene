#ifndef OBSCENEINTERNALS_H_INCLUDED
#define OBSCENEINTERNALS_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

#define OBC_ENUM int

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

enum OBC_QUERY_CONSTS{
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

    //special query things
    OBC_VALUE_CONST_,

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

} OBC_QUERY_CONSTS;





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


#endif // OBSCENEINTERNALS_H_INCLUDED
