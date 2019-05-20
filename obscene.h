#ifndef OBSCENE_H_INCLUDED
#define OBSCENE_H_INCLUDED

#include <stdarg.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

void *OBC_traverseLinkedNodes(void *base, void *node);

//uses a char* as the base, as the size is the smallest primitive type to support as many systems as possible -- 8bit systems and up
//will throw a ptr casting warning in a compiler when the sizeof(char) is smaller than the sizeof(void *)
void* insertLinkedList_(void *base, void *node, void *dataBaseToAdd, ...);

enum{
    COMPARE_ACTION,
    COMPARE_ACTION_END,

    COMPARE_FLOAT_,
    COMPARE_DOUBLE_,
    COMPARE_CHAR_,
    COMPARE_UCHAR_,
    COMPARE_SHORT_,
    COMPARE_USHORT_,
    COMPARE_INT_,
    COMPARE_UINT_,
    COMPARE_LLINT_,
    COMPARE_ULLINT_,
    COMPARE_CHARSTRING_,

    COMPARE_L_,
    COMPARE_G_,
    COMPARE_E_,
    COMPARE_LE_,
    COMPARE_GE_,
    COMPARE_NE_,

    OBC_INSERT_,
    OBC_PREPEND_,
    OBC_APPEND_,
    OBC_MIN_,
    OBC_MAX_,
    OBC_REVERSE_,

    OBC_NODE_

};

#define COMPARE_FLOAT COMPARE_ACTION, COMPARE_FLOAT_
#define COMPARE_DOUBLE COMPARE_ACTION, COMPARE_DOUBLE_
#define COMPARE_CHAR COMPARE_ACTION, COMPARE_CHAR_
#define COMPARE_UCHAR COMPARE_ACTION, COMPARE_UCHAR_
#define COMPARE_SHORT COMPARE_ACTION, COMPARE_SHORT_
#define COMPARE_USHORT COMPARE_ACTION, COMPARE_USHORT_
#define COMPARE_INT COMPARE_ACTION, COMPARE_INT_
#define COMPARE_UINT COMPARE_ACTION, COMPARE_UINT_
#define COMPARE_LLINT COMPARE_ACTION, COMPARE_LLINT_
#define COMPARE_ULLINT COMPARE_ACTION, COMPARE_ULLINT_
#define COMPARE_CHARS8 COMPARE_ACTION, COMPARE_CHARSTRING_, 8
#define COMPARE_CHARS16 COMPARE_ACTION, COMPARE_CHARSTRING_, 16
#define COMPARE_CHARS24 COMPARE_ACTION, COMPARE_CHARSTRING_, 24
#define COMPARE_CHARS32 COMPARE_ACTION, COMPARE_CHARSTRING_, 32

#define COMPARE_L COMPARE_ACTION, COMPARE_L
#define COMPARE_G COMPARE_ACTION, COMPARE_G
#define COMPARE_E COMPARE_ACTION, COMPARE_E
#define COMPARE_LE COMPARE_ACTION, COMPARE_LE
#define COMPARE_GE COMPARE_ACTION, COMPARE_GE

#define COMPARE_END COMPARE_ACTION, COMPARE_ACTION_END

#define insertLinkedList(basePtr, nodePtr, dataBaseToAddPtr, args...) insertLinkedList_(base, node, dataBaseToAddPtr, args, COMPARE_END)

#endif // OBSCENE_H_INCLUDED
