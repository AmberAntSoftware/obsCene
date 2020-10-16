#include "stack.h"

void **OBC_newStack(size_t unitSize){

    OBC_Stack *stack = calloc(1, sizeof(OBC_Stack));
    if(stack == NULL){
        return NULL;
    }

    if(OBC_initStack(stack, unitSize) == OBC_ERROR_FAILURE){
        free(stack);
        return NULL;
    }

    return OBC_StackGetDataPointer(stack);

}
OBC_ERROR_ENUM OBC_initStack(OBC_Stack *stack, size_t unitSize){

    if(OBC_initRayDynamic(&stack->stack, 0, unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}
void **OBC_StackGetDataPointer(OBC_Stack *stack){
    return OBC_RayGetDataPointer(&stack->stack);
}

void OBC_freeStack(void *arr){

    OBC_Stack *stack = OBC_TO_STACK_PTR(arr);
    OBC_freeStackData(stack);
    free(stack);
}
void OBC_freeStackData(OBC_Stack *stack){
    OBC_freeRayData(&stack->stack);
}

OBC_Offset OBC_StackPushRaw(OBC_Stack *stack){
    return OBC_RayNewElementRaw(&stack->stack);
}

OBC_Offset OBC_StackPush(void *arr){
    return OBC_RayNewElement(arr);
}

OBC_Offset OBC_StackPopRaw(OBC_Stack *stack){

    if(stack->stack.curUnitLength > 0){
        OBC_RayPopElementRaw(&stack->stack);
    }

    return stack->stack.curUnitLength;
}

OBC_Offset OBC_StackPop(void *arr){

    OBC_Stack *stack = OBC_TO_STACK_PTR(arr);
    return OBC_StackPopRaw(stack);
}

OBC_ERROR_ENUM OBC_ListPushNewRaw(OBC_Stack *stack, void *item){

    OBC_Offset index = OBC_StackPushRaw(stack);

    if(index == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    size_t unitSize = OBC_RayGetUnitSizeRaw(&stack->stack);
    memcpy(stack->stack.rawData + (index * unitSize), item, unitSize);
    //OBC_X_MEMCPY(stack->stack.rawData,stack->stack.unitSize,data,item);

    return OBC_ERROR_SUCCESS;
}
OBC_ERROR_ENUM OBC_ListPushNew(void *arr, void *item){

    OBC_Stack *stack = OBC_TO_STACK_PTR(arr);
    return OBC_ListPushNewRaw(stack, item);
}

OBC_Offset OBC_StackIterStartRaw(OBC_Stack *stack){

    if(stack->stack.curUnitLength == 0){
        return OBC_NULL_INDEX;
    }else{
        return 0;
    }
}

OBC_Offset OBC_StackIterStart(void *arr){

    OBC_Stack *stack = OBC_TO_STACK_PTR(arr);
    return OBC_StackIterStartRaw(stack);
}

OBC_Offset OBC_StackIterNextRaw(OBC_Stack *stack, OBC_Offset index){

    index++;
    if(stack->stack.curUnitLength <= index){
        return OBC_NULL_INDEX;
    }

    return index;
}

OBC_Offset OBC_StackIterNext(void *arr, OBC_Offset index){

    OBC_Stack *stack = OBC_TO_STACK_PTR(arr);
    return OBC_StackIterNextRaw(stack, index);
}
