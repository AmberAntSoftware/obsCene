#include "obscene.h"

#define DEREF_VOID(ptr) (*((void **) ( ptr ) ))



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
        ret = accumulator;
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

    while(ptr2!=NULL){

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
        COMPARE__DUMP_FULL_SWITCH(type,comparator,EXIT_TYPES,truth, ptr1, ptr2);

    }

    return base;
}
