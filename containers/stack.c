#include "stack.h"

void **OBC_newStack(size_t unitSize){

    OBC_Stack *stack = calloc(1,sizeof(OBC_Stack));
    if(stack == NULL){
        return NULL;
    }

    if(OBC_initStack(stack,unitSize) == OBC_ERROR_FAILURE){
        free(stack);
        return NULL;
    }

    return OBC_StackGetDataPointer(stack);

}
OBC_ERROR_ENUM OBC_initStack(OBC_Stack *stack, size_t unitSize){

    if(OBC_initRayComplex(& stack->stack,0,unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}
void **OBC_StackGetDataPointer(OBC_Stack *stack){
    return (void **)OBC_FROM_RAY_VAL(stack->stack);
}

void OBC_freeStack(void *raw){
    OBC_Stack *stack = OBC_TO_STACK_PTR(raw);
    OBC_freeStackData(stack);
    free(stack);
}
void OBC_freeStackData(OBC_Stack *stack){
    OBC_freeRayData(& stack->stack);
}

OBC_Offset OBC_StackPushRaw(OBC_Stack *stack){
    return OBC_RayNewElement(OBC_FROM_RAY_VAL(stack->stack));
}
OBC_Offset OBC_StackPush(void *raw){
    return OBC_RayNewElement(raw);
}

OBC_Offset OBC_StackPopRaw(OBC_Stack *stack){
    if(stack->stack.curUnitLength > 0){
        OBC_RayPopElement(OBC_FROM_RAY_VAL(stack->stack));
    }
    return stack->stack.curUnitLength;
}
OBC_Offset OBC_StackPop(void *raw){
    return OBC_StackPopRaw(OBC_TO_STACK_PTR(raw));
}

OBC_ERROR_ENUM OBC_ListPushNewRaw(OBC_Stack *stack, void *item){
    size_t data = OBC_StackPushRaw(stack);

    OBC_ERROR_PROPAGATE(data);

    OBC_X_MEMCPY(stack->stack.rawData,stack->stack.unitSize,data,item);

    return OBC_ERROR_SUCCESS;
}
OBC_ERROR_ENUM OBC_ListPushNew(void *raw, void *item){
    return OBC_ListPushNewRaw(OBC_TO_STACK_PTR(raw), item);
}

OBC_Offset OBC_StackIterStartRaw(OBC_Stack *stack){
    if(stack->stack.curUnitLength == 0){
        return OBC_NULL_INDEX;
    }else{
        return 0;
    }
}

OBC_Offset OBC_StackIterStart(void *raw){
    return OBC_StackIterStartRaw(OBC_TO_STACK_PTR(raw));
}

OBC_Offset OBC_StackIterNextRaw(OBC_Stack *stack, OBC_Offset iter){
    iter++;
    if(stack->stack.curUnitLength <= iter){
        return OBC_NULL_INDEX;
    }
    return iter;
}

OBC_Offset OBC_StackIterNext(void *raw, OBC_Offset iter){
    return OBC_StackIterNextRaw(OBC_TO_STACK_PTR(raw), iter);
}
