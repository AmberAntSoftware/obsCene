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

    char *ptr1 = (char*)(ddata-dataOffset);
    char *ptr2 = (char*)(((char*)*base)-dataOffset);
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

        ptr1 = (char*)(ddata-dataOffset);
        ptr2 = (char*)(accumulator-dataOffset);
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

//void *OBC_LinkedListInsert(void **base, void *node, void *cmprNode, void *data, OBC_ENUM type, OBC_ENUM compare){

void *OBC_ArrayPrepend(void *base, void *data, size_t unitSize, size_t arrLen){
    //base+=unitSize;
    memmove((char*)(((char*)base)+unitSize),base,arrLen*unitSize);
    if(unitSize > 32){
        memmove(base,data,unitSize);
        return base;
    }

    if(unitSize-- > 0){
        do{
            *((char*)base) = *((char*)data);
            base = ((char*)base)+1;
            data = ((char*)data)+1;
        }while(unitSize--);
    }

    /*void *offset = (void*)(((char*)NULL)-(char*)(node-*base));
    *((void**)(data-offset)) = *base;
    *base = data;*/

	return base;
}


void *OBC_ArrayInsert(void *base, void *cmprNode, void *data, size_t unitSize, size_t arrLen, OBC_ENUM type, OBC_ENUM compare){

    if(arrLen == 0){
        memmove(base,data,unitSize);
        return base;
    }

    size_t bLen = arrLen*unitSize;
    size_t aLen = arrLen;

    size_t dataOffset = cmprNode-base;
    char truth;
    char *ptr1 = ((char*)data)+dataOffset;
    char *ptr2 = ((char*)base)+dataOffset;

    char *end  = ((char*)base)+bLen;
    char *left = base;


    /*for(;start<end;start+=unitSize){

    }*/

    ///ln(arrLen)/ln(2)+1
    ///check end if uneven
    if( (arrLen & 1) == 1){
        end-=unitSize;
        end+=dataOffset;
        COMPARE__DUMP_FULL_SWITCH(type,compare,EXIT_INSIDE_SWITCH,truth,ptr1,end);
        end-=dataOffset;
        end+=unitSize;
        if(!truth){
            if(unitSize-- > 0){
                do{
                    *end = *((char*)data);
                    end++;
                    data=(char*)(((char*)data)+1);
                }while(unitSize--);
            }
            return base;
        }
    }

    size_t accumuLen = (arrLen)*unitSize;
    if( (arrLen & 1) == 1){
        accumuLen-=unitSize;
    }
    do{
        accumuLen/=2;
        left+=accumuLen;
        left+=dataOffset;
        COMPARE__DUMP_FULL_SWITCH(type,compare,EXIT_LOG_SWITCH,truth,ptr1,left);
        left-=dataOffset;
        if(truth){
            left-=accumuLen;
        }else{
            left+=accumuLen;
        }
    }while(accumuLen>unitSize);


    left+=dataOffset;
    COMPARE__DUMP_FULL_SWITCH(type,compare,EXIT_END_SWITCH_PROCESS,truth,ptr1,left);
    left-=dataOffset;
    if(!truth){
        left+=unitSize;
    }

    //
    memmove(left+unitSize,left,(size_t)(end-left));
    //

    if(unitSize-- > 0){printf("BBBBBBBBBBBBBBBBBBBBB\n");printf("DATA: %i\n",*(int*)data);printf("DATA: %i\n",*(int*)left);printf("DIFF: %i\n",(size_t)(end-left));
        do{
            *left = *((char*)data);
            left++;
            data = (char*)(((char*)data)+1);
        }while(unitSize--);
    }

/*
    ///FIXME dowhile pipeline skip
    //if / dowhile
	while(accumulator!=NULL){

        ptr1 = (char*)(ddata-dataOffset);
        ptr2 = (char*)(accumulator-dataOffset);
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

*/
	return base;
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


/*
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
* /



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
                case():continue;* /
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
    ** /

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
        EXIT_TYPES:;** /
        //COMPARE__DUMP_FULL_SWITCH(type,comparator,EXIT_TYPES,truth, ptr1, ptr2);

    //}

    return pbase;
}
*/

///limit of 127 actual VARGS due to C spec, attempt to minimize fluff besides passed data
/*
void *OBC_query (void *pbase, OBC_ENUM pbaseType, ...){

    int units;

    void *storage[127];//used to store last node path to prevent recalculations -- benefits the preprocess optimizer
    char rawbuffer[32];//for the accumulator types, -- int double float -- ect Avg, Total
    int arrayPos = 0;//replace system with static pointer -- less multiplying
    int arrayLength = 0;
    int unitSize = 1;//for arrays

    char truth = 0;
    char *lastIter = NULL;
    char *currIter = pbase;

    va_list params;
    va_start(params, pbaseType);


    /************************************************************************** /
    ///preprocess into void ptrs and optimize offsets

    //OBC_ENUM dat = 0;
    OBC_ENUM action;

    char* toStore = &(storage[0]);

    //default first param storage
    *((void**)toStore) = (void*)params; toStore+=sizeof(void *);

    do{

        //param count of passed args list
        units = (size_t)(va_arg(params,int));
        *((void**)toStore) = (void*)params; toStore+=sizeof(void *);

        //the specified action of the args list
        action = (va_arg(params,int));
        *((void**)toStore) = (void*)params; toStore+=sizeof(void *);

        //exit if the action is the end token
        if(action == OBC_ACTION_END_){
            break;
        }

        switch(units){
            TOP_OF_PREPROCESS_VARG_SWITCH:
            case(12):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(11):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(10):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(9):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(8):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(7):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(6):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(5):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(4):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(3):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(2):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(1):va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
            case(0):break;
            default:
                for(;units>12;units--){
                    va_arg(params,void*); *((void**)toStore) = (void*)params; toStore+=sizeof(void *);
                }
                goto TOP_OF_PREPROCESS_VARG_SWITCH;
        }

        //exit only by end token
    }while(1);//(dat!=OBC_ACTION_END_);

    //tokens have been parsed into the void* list, use to speed up processing instead of VA_ funcs (tiny possibility of bit packing, choice of error and fix if this has issues)

    toStore = &(storage[0]);

    //init data here for faster query processing
    units = *(int*)toStore; toStore+=sizeof(void *);

    switch(*(int*)toStore){
        case(OBC_QUERY_LINKED_LIST_):
            break;
        case(OBC_QUERY_PTR_ARRAY_):
            break;
        case(OBC_QUERY_RAW_ARRAY_):
            break;
        case(OBC_QUERY_LINKED_ARR_PTR_):
            break;
        case(OBC_QUERY_LINKED_ARR_RAW_):
            break;
    }
    // this should already skip to end when processing, but full skipping for now
    toStore+=sizeof(void *)*units;

    do{
    //get the unit count for the query and advance to actual args
    units = *(int*)toStore; toStore+=sizeof(void *);

    //the specified action of the args list and advance to the "next" args
    action = *(int*)toStore; toStore+=sizeof(void *);

    //exit if the action is the end token
    if(action == OBC_ACTION_END_){
        break;
    }

    toStore+=sizeof(void *)*units;//skip to end to melt pointers backwards efficiently

    switch(*(int*)toStore){
        TOP_OF_PREPROCESS_VOIDP_SWITCH:
        case(12): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(11): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(10): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(9): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(8): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(7): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(6): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(5): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(4): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(3): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(2): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(1): *((void**)toStore) = (*((void**)toStore)) - pbase; toStore-=sizeof(void *);
        case(0):break;
        default:
            for(;units>12;units--){
                *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
            }
            goto TOP_OF_PREPROCESS_VOIDP_SWITCH;
    }
    toStore+=sizeof(void *)*units;//increment for counter size of arguments had as working backwards costs the total amount needing to be skipped

    }while(1);

    /************************************************************************** /
    //finished optimizing looping of data



}*/
void *OBC_query (void *pbase, OBC_ENUM pbaseType, ...){

    int units;

    void *storage[127];//used to store last node path to prevent recalculations -- benefits the preprocess optimizer
    char rawbuffer[32];//for the accumulator types, -- int double float -- ect Avg, Total
    int arrayPos = 0;//replace system with static pointer -- less multiplying
    int arrayLength = 0;
    int unitSize = 1;//for arrays

    char truth = 0;
    char *lastIter = NULL;
    char *currIter = pbase;

    va_list params;
    va_start(params, pbaseType);


    /**************************************************************************/
    ///preprocess into void ptrs and optimize offsets

    //OBC_ENUM dat = 0;
    OBC_ENUM action;

    char* toStore = &(storage[0]);

    ///default first param storage
    ///*((void**)toStore) = (void*)va_arg(params,void *); toStore+=sizeof(void *);

    void *prevPtr;
    void *prevPtr2;

    do{

        //param count of passed args list
        units = (size_t)(va_arg(params,int));
        *(int *)toStore = units; toStore+=sizeof(void *);

        //the specified action of the args list
        action = (va_arg(params,int));
        *(int *)toStore = action; toStore+=sizeof(void *);

        //exit if the action is the end token

        if(action == OBC_ACTION_END_){
            break;
        }

        if(action == OBC_VALUE_CONST_){//do NOT modify the value of the thing
            *((void**)toStore) = (void*)va_arg(params,void*);
            continue;
        }

        prevPtr = pbase;
        for(;units>0;units--){
            *((void**)toStore) = (void*)va_arg(params,void*);//(void*)(((void*)va_arg(params,void*))-prevPtr);
            prevPtr2 = (*((void**)toStore))-prevPtr;
            prevPtr = *((void**)toStore);
            *((void**)toStore) = prevPtr2;
            toStore+=sizeof(void *);
        }

        //exit only by end token
    }while(1);//(dat!=OBC_ACTION_END_);
/**
    //tokens have been parsed into the void* list, use to speed up processing instead of VA_ funcs (tiny possibility of bit packing, choice of error and fix if this has issues)

    toStore = &(storage[0]);

    //init data here for faster query processing
    units = *(int*)toStore; toStore+=sizeof(void *);

    switch(*(int*)toStore){
        case(OBC_QUERY_LINKED_LIST_):
            break;
        case(OBC_QUERY_PTR_ARRAY_):
            break;
        case(OBC_QUERY_RAW_ARRAY_):
            break;
        case(OBC_QUERY_LINKED_ARR_PTR_):
            break;
        case(OBC_QUERY_LINKED_ARR_RAW_):
            break;
    }
    // this should already skip to end when processing, but full skipping for now
    toStore+=sizeof(void *)*units;

    do{
    //get the unit count for the query and advance to actual args
    units = *(int*)toStore; toStore+=sizeof(void *);

    //the specified action of the args list and advance to the "next" args
    action = *(int*)toStore; toStore+=sizeof(void *);

    //exit if the action is the end token
    if(action == OBC_ACTION_END_){
        break;
    }

    toStore+=sizeof(void *)*units;//skip to end to melt pointers backwards efficiently

    switch(*(int*)toStore){
        TOP_OF_PREPROCESS_VOIDP_SWITCH:
        case(12): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(11): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(10): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(9): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(8): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(7): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(6): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(5): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(4): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(3): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(2): *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
        case(1): *((void**)toStore) = (*((void**)toStore)) - pbase; toStore-=sizeof(void *);
        case(0):break;
        default:
            for(;units>12;units--){
                *((void**)toStore) = (*((void**)toStore)) - (*((void**)(toStore-sizeof(void *)))); toStore-=sizeof(void *);
            }
            goto TOP_OF_PREPROCESS_VOIDP_SWITCH;
    }
    toStore+=sizeof(void *)*units;//increment for counter size of arguments had as working backwards costs the total amount needing to be skipped

    }while(1);
*/
    /**************************************************************************/
    //finished optimizing looping of data



}
