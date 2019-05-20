#include "obscene.h"

#define DEREF_VOID(ptr) (*((void **) ( ptr ) ))

void *traverseLinkedNodes(void *base, void *node){
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