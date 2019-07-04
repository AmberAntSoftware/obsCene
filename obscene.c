#include "obscene.h"

#define DEREF_VOID(ptr) ((void *)(*((void **) ( ptr ) )))

void *OBC_LinkedListAppend(void *base, void *node, void *data){
    char *accumulator = node;
    char *ret = node;
    char *offset = (char*)(NULL-(node-base));
	while((accumulator=(char *)DEREF_VOID(accumulator))!=NULL){
        ret = accumulator;
        accumulator -= offset;
	}

	*(void **)(ret-offset) = data;

	return ret;//returns previous as may need previous for other linkage -- do minimum compute
}

void *OBC_LinkedListInsert(void **base, void *node, void *cmprNode, void *data, OBC_ENUM type, OBC_ENUM compare){
    char *accumulator = *base;
    char *ret = *base;
    char *offset = (char*)(((char*)NULL)-(char*)(node-*base));
    char *dataOffset = (char*)(((char*)NULL)-(char*)(cmprNode-*base));
    char *ddata = data;

    char truth;

    char *ptr1 = (char*)(((char*)*base)-dataOffset);
    char *ptr2 = (char*)(ddata-dataOffset);
    COMPARE__DUMP_FULL_SWITCH(type,compare,EXIT_SWITCH,truth,ptr1,ptr2);

    if(truth){

        ///OBC_LinkedListPrepend(base,node,ddata);

        *(char**)(ddata-offset) = *base;
        *base = ddata;

        return;
    }

    accumulator-=offset;
    accumulator=DEREF_VOID(accumulator);

    ///FIXME dowhile pipeline skip
    //if / dowhile
	while(accumulator!=NULL){

        ptr1 = (char*)(accumulator-dataOffset);
        ptr2 = (char*)(ddata-dataOffset);
        COMPARE__DUMP_FULL_SWITCH(type,compare,EXIT_INSIDE_SWITCH,truth,ptr1,ptr2);
        if(truth){

            *((char **)(ddata-offset)) = accumulator;
            *((char **)(ret-offset)) = ddata;

            return;//goes to default assignments
        }

        ret = accumulator;
        accumulator=((char *)(accumulator-offset));
        accumulator=(char *)DEREF_VOID(accumulator);
	}

	*((char **)(ddata-offset)) = accumulator;
    *((char **)(ret-offset)) = ddata;


	return *base;
}

void *OBC_LinkedListRemove(void **base, void *node, void *data){
    char *accumulator = *base;
    char *ret = *base;
    char *offset = (char*)(((char*)NULL)-(char*)(node-*base));
    char *ddata = data;

    char truth;

    char *ptr1 = *base;
    char *ptr2 = ddata;
    COMPARE__DUMP_FULL_SWITCH(OBC_ACTION_COMPARE_NODE_,OBC_COMPARE_E_,EXIT_SWITCH,truth,ptr1,ptr2);

    if(truth){

        ret = *base;
        *base = *(char**)(((char*)*base)-offset);

        return ret;
    }

    accumulator-=offset;
    accumulator=DEREF_VOID(accumulator);

    ///FIXME dowhile pipeline skip
    //if / dowhile
	while(accumulator!=NULL){

        ptr1 = accumulator;
        //ptr2 = (char*)(ddata);
        COMPARE__DUMP_FULL_SWITCH(OBC_ACTION_COMPARE_NODE_,OBC_COMPARE_E_,EXIT_INSIDE_SWITCH,truth,ptr1,ptr2);
        if(truth){

            *((char **)(ret-offset)) = *((char **)(accumulator-offset));

            return accumulator;//goes to default assignments
        }

        ret = accumulator;
        accumulator=((char *)(accumulator-offset));
        accumulator=(char *)DEREF_VOID(accumulator);
	}

	return NULL;
}

void *OBC_LinkedListPrepend(void **base, void *node, void *data){

    void *offset = (void*)(((char*)NULL)-(char*)(node-*base));
    *((void**)(data-offset)) = *base;
    *base = data;

	return *base;
}







void *OBC_traverseLinkedNodes(void *base, void *node){
    void *accumulator = node;
    void *ret = node;
    /*/
    void *offset = (void*)((node-base)-1);
    void *max = (void*)(((void *)0)-1);
    /*/
    void *offset = (void*)((node-base));
    void *max = NULL;
    //*/
	while((accumulator=(void *)DEREF_VOID(accumulator))!=NULL){
        ret = accumulator;printf("H: %p\n",accumulator);
        accumulator -= max-offset;
	}

	return ret;
}





void* linkedListTraversal_(void *base, ...){
    char* buffer;

    char truth = 0;

    int type = 0;
    int comparator = 0;
    char inversion = 0;
    void *ptr1;
    void *ptr2;

    int argumentCount = 0;

    va_list params;
    va_start(params, base);
    ///

    ///FIXME USE BETTER NAMES FOR ENDING TOKENS
    ///OPTIMIZING ROUND -- combine node data here
    while(1){
        ptr1 = va_arg ( params, void* );
        printf("ARG\n");

        if(ptr1 == OBC_ACTION_){
            ptr1 = va_arg ( params, void* );
            switch((unsigned int)ptr1){
                case(OBC_ACTION_END_):
                    goto EXIT_PREPROCESS;
                /*case(OBC_ACTION_NEXTNODE_):
                    continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;*/
                //
                case(OBC_ACTION_NEXTNODE_):
                    //process nodes
                    continue;
                case(OBC_ACTION_DATANODE_):
                    //process nodes
                    continue;

            }
        }
    }
    EXIT_PREPROCESS:

    printf("O K\n");
    return NULL;

    ///
    va_end(params);

    /**
    ***
    ***
    ***
    ***
    **/

    //while(ptr2!=NULL){

        /**
        switch(type){
            case(OBC_ACTION_COMPARE_INT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_UINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_FLOAT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, float, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_DOUBLE_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, double, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_CHAR_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed char, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_UCHAR_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned char, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_SHORT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed short, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_USHORT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned short, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_LLINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed long long int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_ULLINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned long long int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_CHARSTRING_):{
                //
            }
        }
        EXIT_TYPES:;**/
        //COMPARE__DUMP_FULL_SWITCH(type,comparator,EXIT_TYPES,truth, ptr1, ptr2);

    //}

    return base;
}



void *OBC_query (void *pbase, OBC_ENUM pbaseType, ...){

    char rawbuffer[32];//for the accumulator types, -- int double float -- ect Avg, Total
    int arrayPos = 0;//replace system with static pointer -- less multiplying
    int arrayLength = 0;
    int unitSize = 1;//for arrays

    char truth = 0;
    char *lastIter = NULL;
    char *currIter = pbase;

    va_list params;
    va_start(params, pbaseType);

    if(pbaseType == OBC_QUERY_HEAP_SPECIFIED_){
        ///TODO
    }

#ifdef GCC
//direct label jumps here
//void *locs[] = {(&OBC_query)+((&OBC_query)- (&&ARR_RAW)),};
#endif // GCC

/*
switch(pbaseType){
case(OBC_QUERY_LINKED_ARR_RAW_):
    unitSize = va_arg(params, int);
case(OBC_QUERY_LINKED_ARR_PTR_):
case(OBC_QUERY_RAW_ARRAY_):
    unitSize = va_arg(params, int);
case(OBC_QUERY_PTR_ARRAY_):
    arrayLength = va_arg(params, int);
}
*/



    ///

    ///FIXME USE BETTER NAMES FOR ENDING TOKENS
    ///OPTIMIZING ROUND -- combine node data here
do{
switch(pbaseType){///TODO getPointers
case(OBC_QUERY_PTR_ARRAY_)://arrayLength storage
    //deref then treat as raw
    currIter = DEREF_VOID(pbase+arrayPos);
case(OBC_QUERY_RAW_ARRAY_)://arrayLength storage
    currIter = &pbase[arrayPos];
    arrayPos++;
    break;
case(OBC_QUERY_LINKED_LIST_):
    break;
case(OBC_QUERY_LINKED_ARR_PTR_)://arrayLength dynamic storage
    break;
case(OBC_QUERY_LINKED_ARR_RAW_)://arrayLength dynamic storage
    break;
}

        currIter = va_arg ( params, OBC_ENUM );
        printf("ARG\n");

        if(currIter == OBC_ACTION_){
            currIter = va_arg ( params, void* );
            switch((unsigned int)currIter){
                case(OBC_ACTION_END_):
                    goto EXIT_PREPROCESS;
                /*case(OBC_ACTION_NEXTNODE_):
                    continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;
                case():continue;*/
                //
                case(OBC_ACTION_NEXTNODE_):
                    //process nodes
                    continue;
                case(OBC_ACTION_DATANODE_):
                    //process nodes
                    continue;

            }
        }

lastIter = currIter;
}while(1);
    ///
    EXIT_PREPROCESS:
    va_end(params);

    /**
    ***
    ***
    ***
    ***
    **/

    //while(ptr2!=NULL){

        /**
        switch(type){
            case(OBC_ACTION_COMPARE_INT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_UINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_FLOAT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, float, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_DOUBLE_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, double, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_CHAR_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed char, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_UCHAR_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned char, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_SHORT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed short, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_USHORT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned short, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_LLINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed long long int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_ULLINT_):
                    COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned long long int, truth, ptr1, ptr2);
            case(OBC_ACTION_COMPARE_CHARSTRING_):{
                //
            }
        }
        EXIT_TYPES:;**/
        //COMPARE__DUMP_FULL_SWITCH(type,comparator,EXIT_TYPES,truth, ptr1, ptr2);

    //}

    return pbase;
}
