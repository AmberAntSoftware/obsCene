#ifndef OBCLARGE_H_INCLUDED
#define OBCLARGE_H_INCLUDED

#include "ray.h"
#include "list.h"
#include "queue.h"
#include "dequeue.h"
#include "stack.h"
#include "hashmap.h"
#include "tree.h"

typedef union OBC_HazardOffset{

    size_t combined;
    OBCL_Offset val;

}OBC_HazardOffset;

typedef union TPoint{

   signed char scx;
   signed char scy;

   unsigned char ucx;
   unsigned char ucy;

   signed int six;
   signed int siy;

   unsigned int uix;
   unsigned int uiy;

   unsigned long ulx;
   unsigned long uly;

   signed long slx;
   signed long sly;

   float fx;
   float fy;

   double dx;
   double dy;

   long double ldx;
   long double ldy;

}TPoint;

typedef union OBC_Wrapper{

    OBC_Ray ray;
    OBC_List list;
    OBC_Dequeue dequeue;
    OBC_Queue queue;
    OBC_HashMap hashmap;
    OBC_Stack stack;
    OBC_BTree tree;

}OBC_Wrapper;











typedef struct OBC_Container{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_Container;

typedef struct OBC_LargeRay{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeRay;

typedef struct OBC_LargeList{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeList;

typedef struct OBC_LargeQueue{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeQueue;

typedef struct OBC_LargeDequeue{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeDequeue;

typedef struct OBC_LargeTree{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeTree;

typedef struct OBC_LargeHashMap{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeHashMap;

typedef struct OBC_LargeStack{

    OBC_Ray indirect;
    OBC_Ray backed;

}OBC_LargeStack;

#endif // OBCLARGE_H_INCLUDED
