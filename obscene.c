#include "obscene.h"

#define DEREF_VOID(ptr) (*((void **) ( ptr ) ))

#define COMPARE__SWITCH_SET(comparator, exitLabel, type, charStorageResult, ptr1, ptr2) \
switch( comparator ){\
    COMPARE__SWITCH_UNIT( COMPARE_E_, exitLabel , type ,== , charStorageResult, ptr1 , ptr2);\
    COMPARE__SWITCH_UNIT( COMPARE_L_, exitLabel , type , <, charStorageResult, ptr1, ptr2);\
    COMPARE__SWITCH_UNIT( COMPARE_LE_, exitLabel , type , <= ,charStorageResult, ptr1 , ptr2);\
    default:goto exitLabel;\
}
#define COMPARE__SWITCH_UNIT(switch_value, exitLabel, type, action, charStorageResult, ptr1, ptr2) \
case(switch_value): \
    charStorageResult = (*(type *) ptr1 ) action (*(type *) ptr2 );\
    goto exitLabel;




void *OBC_traverseLinkedNodes(void *base, void *node){
	void *offset = (void*)((node-base)-1);
    void *accumulator = node;
    void *ret = node;
    void *max = (void*)(((void *)0)-1);

	while((accumulator=(void *)DEREF_VOID(accumulator))!=NULL){
        ret = accumulator;
        accumulator -= max-offset;
	}

	return ret;
}




#define COMPARE_END COMPARE_ACTION, COMPARE_ACTION_END

void* insertLinkedList_(void *base, void *node, void *dataBaseToAdd, ...){
    char* buffer;

    int truth = 0;

    int type = 0;
    int comparator = 0;
    char inversion = 0;
    void *ptr1;
    void *ptr2;
    switch(type){
        case(COMPARE_INT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed int, truth, ptr1, ptr2);
        case(COMPARE_UINT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned int, truth, ptr1, ptr2);
        case(COMPARE_FLOAT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, float, truth, ptr1, ptr2);
        case(COMPARE_DOUBLE_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, double, truth, ptr1, ptr2);
        case(COMPARE_CHAR_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed char, truth, ptr1, ptr2);
        case(COMPARE_UCHAR_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned char, truth, ptr1, ptr2);
        case(COMPARE_SHORT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed short, truth, ptr1, ptr2);
        case(COMPARE_USHORT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned short, truth, ptr1, ptr2);
        case(COMPARE_LLINT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, signed long long int, truth, ptr1, ptr2);
        case(COMPARE_ULLINT_):
                COMPARE__SWITCH_SET(comparator, EXIT_TYPES, unsigned long long int, truth, ptr1, ptr2);
        case(COMPARE_CHARSTRING_):{
            //
        }
    }
    EXIT_TYPES:

    /*buffer = SDL_malloc(sizeof(char));
    if(buffer==NULL){
        return NULL;
    }
    buffer[0] = '\0';


    va_list params;
    va_start(params, char_pointer_count);
    ///

    int i;
    for(i = 0; i < char_pointer_count;i++){
        buffer = STR_concatf2(buffer,va_arg ( params, char* ));
    }
    ///
    va_end(params);*/

    return base;
}
