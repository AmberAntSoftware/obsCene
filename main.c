#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obc_stdlib/memswap.h"

#include "obc.h"

#include "containers/ray.h"
#include "allocators/allocator.h"
#include "allocators/alloclist.h"

#include "containers/list.h"
#include "containers/stack.h"
#include "containers/queue.h"
#include "containers/tree.h"
#include "containers/hashmap.h"
#include "allocators/allocray.h"

#include "allocators/alloclistbit.h"

#include "obc_stdlib/hash.h"
#include "obc_stdlib/random.h"

typedef struct BIG_DATA{
    unsigned char dump[180];
}BIG_DATA;

typedef struct Point{
    size_t x;
    size_t y;
}Point;


const size_t ALLOC_SIZE = (1<<23);

typedef struct A{
    int we34;
    int ka;
    int kl;
    int baz[5];
}A;

typedef struct STR_String{
    size_t length;
    char *data;
    char small[32];
}STR_String;

#define toTest A

toTest rgei;

int allocatorStressTest()
{
    /*
    printf("Hello world!\n");
    srand(time(NULL));

    int i;
    A **ray = (A**)OBC_newRay(1,sizeof(A));

    A rgei;
    rgei.kl = 77;

    for(i = 0; i < 10; i++){
        rgei.kl=i;
        if((rand()&1)==1){
            OBC_RayAddPointer(ray,&rgei);
        }else{
            OBC_RayAddValue(ray,rgei);
        }
    }

    printf("BSize: %i\n",OBC_RayLength(ray));
    printf("Size: %i\n",OBC_RayUnitLenth(ray));

    for(i = 0; i < OBC_RayUnitLenth(ray); i++){
        printf("VALUE: %i\n",OBC_RayGetValue(ray,i).kl);
    }

    OBC_RayRemove(ray,rand()%10);
    OBC_RayRemoveFast(ray,rand()%9);
    printf("===============REMOVE================\n");
    for(i = 0; i < OBC_RayUnitLenth(ray); i++){
        printf("VALUE: %i\n",OBC_RayGetValue(ray,i).kl);
    }*/

    /**toTest **ray = (toTest**)OBC_newRay(10000000,sizeof(toTest));

    OBC_Ray *cheat = OBC_TO_RAW_RAY(ray);
    int i = 0;

    int *ttest = NULL;**/
    /**
    for(i = 0; i < 0; i++){
        //OBC_RayAddValue(ray,rgei);
        /*
        if(OBC_TO_RAW_RAY(ray)->curLength==OBC_TO_RAW_RAY(ray)->maxLength){
            if(OBC_RayExpand(ray) == OBC_FAILURE){
                break;
            }
        }
        /* /
        OBC_RayDoExpand(ray);
        //* /
        (*ray)[i]=rgei;
        OBC_TO_RAW_RAY(ray)->curLength+=OBC_TO_RAW_RAY(ray)->unitSize;
        OBC_TO_RAW_RAY(ray)->unitLength++;
        //OBC_RayNewElement(ray);
    }
    **/
    /**
    for(i = 0; i < 0; i++){
        //*
        OBC_RayAddValue(ray,rgei);
        /* /
        if(OBC_RayDoExpand(ray)!= OBC_FAILURE){
            (*(ray))[OBC_TO_RAW_RAY(ray)->unitLength] = rgei;
            //OBC_RayPushElement(ray);
            OBC_TO_RAW_RAY(ray)->unitLength++;
            OBC_TO_RAW_RAY(ray)->maxLength+=OBC_TO_RAW_RAY(ray)->unitSize;
        }
        //* /
    }
    //**/

    /*
    STR_String **sray = OBC_newRay(40000000,sizeof(STR_String));

    STR_String cheekyAF;

    for(i = 0; i < 40000000; i++){
        OBC_RayAddValue(sray,cheekyAF);
    }
    //*/

    /**
    for(i = 0; i < 10000000; i++){
        OBC_RayAddValue(ray,rgei);
    }
    /**/
    A **datas = (A**)OBC_newAllocRay(sizeof(A));
    size_t prev = 0;
    size_t cur = 0;
    /**
    printf("%i\n",i);
    printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)cur,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->backed.maxUnitLength);

        cur = OBC_AllocRayMalloc(datas);
        //printf("%i\n",i);
        //printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)cur,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->backed.maxUnitLength);
        if(cur-prev > 1){
            printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)cur,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->backed.maxUnitLength);
            printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)prev,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->meta.maxUnitLength);
        }

        if(i%100000 == 0){
            printf("%i \n",i);
        }

        prev = cur;
    }

    OBC_AllocRayFree(datas,8742);
    OBC_T ret = OBC_AllocRayMalloc(datas);
    if(ret == 8742){
        printf("SUCCESS\n");
    }else{
        printf("FAILURE %llu\n",(unsigned long long int)ret);
    }

    /*/
    datas = (A**)OBC_newAllocator(sizeof(A));
    OBC_Allocator *dump = OBC_TO_RAW_ALLOCATOR(datas);
    //datas = OBC_newRay(8,sizeof(A));
    prev = 0;
    size_t i;
    printf("START\n");
    volatile clock_t t1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        //**
            //printf("%i\n",i);
        cur = OBC_AllocatorMalloc(datas);
        OBC_AllocatorGetValue(datas,cur)=rgei;
        //A *data = malloc(sizeof(A));
        //printf("%i\n",i);
        //printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)cur,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->backed.maxUnitLength);
/*        if(i > 1020){
        OBC_Allocator2 *allocator_ = datas;//OBC_TO_RAW_ALLOCATOR2(((void*)datas));
printf("/////%i:::  CUR : %llu    PREV:%llu\n",i, (long long unsigned int)cur,(long long unsigned int)prev);
        }
//*/
        if(cur-prev != 1 ){
            ///printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)cur,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->backed.maxUnitLength);
            ///printf("%i DAT: %llu  || %llu\n",i,(long long unsigned int)prev,(long long unsigned int)(OBC_TO_RAW_ALLOCRAY(datas))->meta.maxUnitLength);
            //OBC_Allocator2 *dump = OBC_TO_RAW_ALLOCATOR2(datas);
            printf("%i:::  CUR : %llu    PREV:%llu\n",i, (long long unsigned int)cur,(long long unsigned int)prev);
            printf("UNITS: %u\n",dump->metaUnits);
            printf("curUnits: %u\n",dump->backed.curUnitLength);

            printf("META_LENGTH %u\n",(int)dump->meta[0].maxUnitLength);
            size_t ii;
            for(ii = 0; ii <dump->meta[0].maxUnitLength;ii++ ){
                printf("%i ",(int)dump->meta[0].rawData[ii]);
            }puts("");

            if(dump->metaUnits){
                printf("BOTTOMER: %i\n",(int)dump->meta[0].rawData[OBC_ALLOC_META_CHUNKSIZE]);
                printf("TOPPER: %i\n",(int)dump->meta[1].rawData[0]);
            }

        }
//printf("/////%i:::  CUR : %llu    PREV:%llu\n",i, (long long unsigned int)cur,(long long unsigned int)prev);
        prev = cur;
        /*/

        OBC_RayNewElement(datas);

        //*/
    }

    long long int accu = 0;
    srand(6464);
    volatile clock_t a1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_AllocatorGetPointer(datas,dar)->we34;
        accu+=OBC_AllocatorGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a2 = clock();


    //**/

    volatile clock_t t2 = clock();

    printf("t_siz  %i:\n",OBC_ALLOC_META_BITS);
    printf("expect_t_siz  %i:\n",sizeof(size_t));
    printf("%i   UNIT %llu\n",0,
           (unsigned long long int)OBC_ALLOC_UNITS_0);
    printf("%i   UNIT %llu\n",1,
           (unsigned long long int)OBC_ALLOC_UNITS_1);
    printf("%i   UNIT %llu\n",2,
           (unsigned long long int)OBC_ALLOC_UNITS_2);
    printf("%i   UNIT %llu\n",3,
           (unsigned long long int)OBC_ALLOC_UNITS_3);


    printf("Time: %f\n",1000.0*(t2-t1)/CLOCKS_PER_SEC);
///OBC_Allocator2Free(datas,65742);
    cur = OBC_AllocatorMalloc(datas);
    printf("GOT: %llu\n",(long long unsigned int)cur);
    cur = OBC_AllocatorMalloc(datas);
    printf("GOT: %llu\n",(long long unsigned int)cur);

    printf("CUR: %p\n",(long long unsigned int)dump->backed.rawData);
    printf("SIZ: %llu\n",(long long unsigned int)dump->metaUnits);
    printf("CUR: %p\n",(long long unsigned int)dump->backed.rawData);

    puts("=============");
    for(i = 0; i <= dump->metaUnits; i++){
        printf("first: %p\n",((void **)dump->meta[i].rawData)[0]);
        printf("first: %i\n",(int)dump->meta[i].rawData[0]);
        printf("second: %i\n",(int)dump->meta[i].rawData[1]);
        //printf("second: %p\n",((void **)dump->meta[i].rawData)[1]);
        //printf("second: %i\n",(int)dump->meta[i].rawData[1]);
    }
    puts("=============");
//OBC_Allocator2MarkAllocated3(datas,8);
///OBC_Allocator2Free(datas,1961);
    for(i = 0; i <= dump->metaUnits; i++){
        printf("first: %p\n",((void **)dump->meta[i].rawData)[0]);
        printf("first: %i\n",(int)dump->meta[i].rawData[0]);
        printf("second: %i\n",(int)dump->meta[i].rawData[1]);
        //printf("second: %p\n",((void **)dump->meta[i].rawData)[1]);
        //printf("second: %i\n",(int)dump->meta[i].rawData[1]);
    }
    /*
    cur = 99;
    printf("SPR_FIRST: %i\n",(int)dump->meta[0].rawData[0]);
    for(i = 0; i < 5; i++){
        printf("SPR__: %i\n",(int)dump->meta[0].rawData[i]);
        if(dump->meta[0].rawData[i]!=~0){
            cur = (size_t)i;
            break;
        }
    }
    /*
    cur = OBC_metaFirstEmptyBit(
        (OBC_ALLOC_META_TYPE *)dump->meta[0].rawData,
        dump->meta[0].maxUnitLength);
    /*/
/**
OBC_Allocator2Free3(datas,6481);
    cur = OBC_metaFirst0Bit(
        (OBC_ALLOC_META_TYPE*)dump->meta[1].rawData,
        64);
    //*/
    printf("PRECUR: %u\n",cur);
    printf("PRECUR_where: %p\n",dump->meta[0].rawData);
    printf("PRECUR_max: %u\n",dump->meta[0].maxUnitLength);
    cur = OBC_AllocatorMalloc(datas);
    printf("CUR: %u\n",cur);
    cur = OBC_AllocatorMalloc(datas);
    printf("CUR: %u\n",cur);


    printf("\n\n\nSTRESS_TESTING\n\n\n");
    ///srand(time(NULL));
    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < dump->backed.curUnitLength; i++){
        size_t dar = rand()%dump->backed.curUnitLength;
        OBC_AllocatorFree(datas,dar);
        if(OBC_AllocatorMalloc(datas) != dar){
            printf("FAIL: %u",dar);
        }
    }
    volatile clock_t t4 = clock();


    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_AllocatorGetPointer(datas,dar)->we34;
        accu+=OBC_AllocatorGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a4 = clock();

    printf("DeAllocating..\n");
    toTest **stores = malloc(ALLOC_SIZE*sizeof(toTest*));
    for(i=0; i < ALLOC_SIZE; i++)
        stores[i] = NULL;
    volatile clock_t t5 = clock();
    OBC_freeAllocator(datas);
    volatile clock_t t6 = clock();


    puts("\n\n");
    printf("AllocCopy Time: %f\n",1000.0*(t2-t1)/CLOCKS_PER_SEC);
    printf("RandomAlloc Time: %f\n",1000.0*(t4-t3)/CLOCKS_PER_SEC);
    printf("Deallocation Time: %f\n",1000.0*(t6-t5)/CLOCKS_PER_SEC);
    printf("InitRead Time: %f\n",1000.0*(a2-a1)/CLOCKS_PER_SEC);
    printf("RandomRead Time: %f\n",1000.0*(a4-a3)/CLOCKS_PER_SEC);

    return 0;
}

int lazyStressTest(){

    long long int accu = 0;

    int size = ALLOC_SIZE;
    toTest **stores = malloc(ALLOC_SIZE*sizeof(toTest*));

    size_t i;

    printf("START\n");
    volatile clock_t t1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        stores[i] = malloc(sizeof(toTest));
        *(stores[i]) = rgei;
        if(stores[i]==NULL){
            printf("FAILURE: %u\n",i);
        }
    }
    volatile clock_t t2 = clock();



    srand(6464);
    volatile clock_t a1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=stores[dar]->we34;
        accu+=stores[dar]->baz[3];
    }
    volatile clock_t a2 = clock();



    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        free(stores[dar]);
        stores[dar] = malloc(sizeof(toTest));
        *(stores[dar]) = rgei;
        if(stores[dar] == NULL){
            printf("FAIL: %u\n",dar);
        }
    }
    volatile clock_t t4 = clock();

    accu=0;
    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=stores[dar]->we34;
        accu+=stores[dar]->baz[3];
    }
    volatile clock_t a4 = clock();


    printf("Deallocation\n");
    volatile clock_t t5 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        free(stores[i]);
    }
    volatile clock_t t6 = clock();

    puts("\n\n");
    printf("AllocCopy Time: %f\n",1000.0*(t2-t1)/CLOCKS_PER_SEC);
    printf("RandomAlloc Time: %f\n",1000.0*(t4-t3)/CLOCKS_PER_SEC);
    printf("Deallocation Time: %f\n",1000.0*(t6-t5)/CLOCKS_PER_SEC);
    printf("InitRead Time: %f\n",1000.0*(a2-a1)/CLOCKS_PER_SEC);
    printf("RandomRead Time: %f\n",1000.0*(a4-a3)/CLOCKS_PER_SEC);

    free(stores);

    return 0;
}

int allocfastTest(){

    size_t i;

    long long int accu = 0;

    int size = ALLOC_SIZE;
    size_t *stores = malloc(ALLOC_SIZE*sizeof(size_t));
    toTest **datas = (toTest**)OBC_newAllocList(sizeof(toTest));


    //*
    printf("START\n");
    volatile clock_t t1 = clock();
    size_t prev = 0,cur = 0;
    for(i = 0; i < ALLOC_SIZE; i++){
        cur = OBC_AllocListMalloc(datas);
        if(cur-prev != 1){
            printf("FAILURE: %u     PREV: %u\n",cur,prev);
        }
        prev = cur;
    }
    volatile clock_t t2 = clock();



    accu=0;
    srand(6464);
    volatile clock_t a1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_AllocListGetPointer(datas,dar)->we34;
        accu+=OBC_AllocListGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a2 = clock();


    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        OBC_AllocListFree(datas,dar);
        size_t allc = OBC_AllocListMalloc(datas);
        if(allc!=dar){
            printf("FAIL_RAND ALLOC: %u     DAR: %u\n",allc,dar);
        }
        OBC_AllocListGetValue(datas,dar) = rgei;
    }
    volatile clock_t t4 = clock();


    accu=0;
    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_AllocListGetPointer(datas,dar)->we34;
        accu+=OBC_AllocListGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a4 = clock();


    printf("Deallocation\n");
    printf("BackedSize: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->backed.curUnitLength);
    printf("BackedNoFreeMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    volatile clock_t t5 = clock();

    for(i = 0; i < ALLOC_SIZE; i++){
        OBC_AllocListFree(datas,i);
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    //*
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t allc = OBC_AllocListMalloc(datas);
        if(allc!=ALLOC_SIZE-i){
            printf("FAIL_ALL ALLOC: %u     EXPECT: %u\n",allc,i);
        }
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    /*/
        OBC_freeAllocFast(datas);
    //*/
    volatile clock_t t6 = clock();

    puts("\n\n");
    printf("AllocCopy Time: %f\n",1000.0*(t2-t1)/CLOCKS_PER_SEC);
    printf("RandomAlloc Time: %f\n",1000.0*(t4-t3)/CLOCKS_PER_SEC);
    printf("Deallocation Time: %f\n",1000.0*(t6-t5)/CLOCKS_PER_SEC);
    printf("InitRead Time: %f\n",1000.0*(a2-a1)/CLOCKS_PER_SEC);
    printf("RandomRead Time: %u\n",a4-a3);

    OBC_AllocList *fast = OBC_TO_RAW_ALLOCLIST(datas);

    printf("BackedDataSize: %u\n",OBC_Ray_CurByteLength(OBC_FROM_RAY_VAL(fast->backed)));
    printf("BackedMetaSize: %u\n",OBC_Ray_CurByteLength(OBC_FROM_RAY_VAL(fast->meta)));
    printf("BackedTotalSize: %u\n",OBC_Ray_CurByteLength(OBC_FROM_RAY_VAL(fast->meta))+OBC_Ray_CurByteLength(OBC_FROM_RAY_VAL(fast->backed)));


    return 0;
    //*/

}




void listTests(){

    STR_String **strs = (STR_String**)OBC_newList(sizeof(STR_String));
    printf("%p\n",strs);
    printf("%u\n",sizeof(OBC_Ray));

    STR_String *data;

    OBC_Offset item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "stuff";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "things";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "forks";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "wkebgkwgek";

    OBC_Offset iter;
    for(iter = OBC_ListIterStart(strs); iter != OBC_NULL_INDEX; iter = OBC_ListIterNext(strs, iter)){

        printf("ITER: %u\n",iter);

        puts( (*strs)[iter].data );

        //data = OBC_GetPointer(strs,iter);
        //printf(data->data);
    }

    OBC_freeList(strs);

    strs = (STR_String**)OBC_newList(sizeof(STR_String));

    STR_String rrgei;



    volatile clock_t start = clock();
    unsigned int i;
    for(i = 0; i < ALLOC_SIZE; i++){
        //*
        item = OBC_ListNewItem(strs);
        ///OBC_GetPointer(strs,item)->length = 15;
        ///OBC_GetPointer(strs,item)->data = "wkebgkwgek";
        ///OBC_GetPointer(strs,item)->small[OBC_GetPointer(strs,item)->length] = 'k';
        /*/
        OBC_ListAddRawFast(strs,rrgei);
        //if(OBC_ListAddRaw(strs,&rrgei)==OBC_ERROR_FAILURE){
        //    puts("FAILURE");
        //}
        //*/
    }
    volatile clock_t end = clock();

    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        OBC_ListRemove(strs,dar);
        size_t allc = OBC_ListNewItem(strs);
        if(allc!=dar){
            printf("FAIL_RAND LIST: %u     EXPECT: %u\n",allc,dar);
        }
    }
    volatile clock_t t4 = clock();


    printf("LIST ADD TIME: %ums\n",OBC_timerMillis(start,end));
    printf("LIST RANDOM ALLOC TIME: %ums\n",OBC_timerMillis(t3,t4));
}

void stackExamples(){

    STR_String **strs = (STR_String **)OBC_newStack(sizeof(STR_String));

    STR_String *data;

    OBC_Offset item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "stuff";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "things";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "forks";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        strs[obc(item)].data = "wkebgkwgek";

    OBC_Offset iter;
    for(iter = OBC_StackIterStart(strs); iter != OBC_NULL_INDEX; iter = OBC_StackIterNext(strs, iter)){

        printf("ITER: %u\n",iter);

        puts( (*strs)[iter].data );

        //data = OBC_GetPointer(strs,iter);
        //printf(data->data);
    }

    OBC_freeStack(strs);

    strs = (STR_String**)OBC_newStack(sizeof(STR_String));

    STR_String rrgei;



    volatile clock_t start = clock();
    unsigned int i;
    for(i = 0; i < ALLOC_SIZE; i++){
        //*
        item = OBC_StackPush(strs);
        strs[obc(item)].length = 15;
        strs[obc(item)].data = "wkebgkwgek";
        strs[obc(item)].small[strs[obc(item)].length] = 'k';
        /*/
        OBC_ListAddRawFast(strs,rrgei);
        //if(OBC_ListAddRaw(strs,&rrgei)==OBC_ERROR_FAILURE){
        //    puts("FAILURE");
        //}
        //*/
    }
    volatile clock_t end = clock();

    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t popped = OBC_StackPop(strs);
        size_t allc = OBC_StackPush(strs);
        if(allc!=popped){
            printf("FAIL_POP LIST: %u     EXPECT: %u\n",allc,popped);
        }
    }
    volatile clock_t t4 = clock();


    printf("STACK ADD TIME: %ums\n",OBC_timerMillis(start,end));
    printf("STACK RANDOM ALLOC TIME: %ums\n",OBC_timerMillis(t3,t4));
}

void queueTests(){

    STR_String **strs = (STR_String**)OBC_newQueue(sizeof(STR_String));

    OBC_Offset item = OBC_QueuePush(strs);
    printf("QItem: %u\n",item);
        (*strs)[item].data = "stuff";
    //*
    item = OBC_QueuePush(strs);
    printf("QItem: %u\n",item);
        (*strs)[item].data = "things";
    item = OBC_QueuePush(strs);
    printf("QItem: %u\n",item);
        (*strs)[item].data = "forks";
    item = OBC_QueuePush(strs);
    printf("QItem: %u\n",item);
        (*strs)[item].data = "wkebgkwgek";
    //*/

    OBC_Offset iter;
    OBC_QueueForEach(strs,iter){

        printf("QIter: %u\n",iter);

        puts( (*strs)[iter].data );

    }

    OBC_freeQueue(strs);
    /*
    strs = OBC_newStack(sizeof(STR_String));

    STR_String rrgei;



    volatile clock_t start = clock();
    unsigned int i;
    for(i = 0; i < ALLOC_SIZE; i++){
        item = OBC_StackPush(strs);
        OBC_GetPointer(strs,item)->length = 15;
        OBC_GetPointer(strs,item)->data = "wkebgkwgek";
        OBC_GetPointer(strs,item)->small[OBC_GetPointer(strs,item)->length] = 'k';
    }
    volatile clock_t end = clock();

    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t popped = OBC_StackPop(strs);
        size_t allc = OBC_StackPush(strs);
        if(allc!=popped){
            printf("FAIL_POP LIST: %u     EXPECT: %u\n",allc,popped);
        }
    }
    volatile clock_t t4 = clock();


    printf("STACK ADD TIME: %ums\n",OBC_timerMillis(start,end));
    printf("STACK RANDOM ALLOC TIME: %ums\n",OBC_timerMillis(t3,t4));*/
}

















void listStuff(){

    Point **points = (Point**)OBC_newList(sizeof(Point));
    srand(time(NULL));

    Point point;
    unsigned int i;
    for(i = 0; i < 32; i++){
        point.x = rand();
        point.y = rand();
        OBC_ListAdd(points, &point);
    }


    OBC_Iterator unit;

    /**************/
    for(unit = OBC_ListIterStart(points); unit != OBC_NULL_INDEX; unit = OBC_ListIterNext(points,unit)){
        printf("Point: %u, %u\n", (*points)[unit].x, (*points)[unit].y);
    }
    //Equivalent
    puts("-------------");
    Point *const pt = *points;
    OBC_ListForEach(points, unit){
        pt[unit].x++;
        printf("Point: %u, %u\n", pt[unit].x, pt[unit].y);
    }

    OBC_ListForEach(points, unit){
        printf("Point: %u, %u\n", (*points)[unit].x, (*points)[unit].y);
    }

    OBC_ListForEach(points, unit){
        printf("Point: %u, %u\n", points[0][unit].x, points[0][unit].y);
    }
    /**************/

    OBC_freeList(points);

}



void treeStuff(){

    /**
    Point **points = OBC_newTree(sizeof(Point));
    srand(time(NULL));

    Point point;
    unsigned int i;

    OBC_Iterator iter;
    OBC_SpaceshipValue cmpr;
    for(i = 0; i < 32; i++){
        point.x = rand();
        point.y = rand();
        OBC_TreeAddLoop(points, &point , iter,cmpr){
            cmpr = point.x - (*points)[iter].x;
        }
    }


    OBC_Iterator unit;

    / ************** /
    for(unit = OBC_ListIterStart(points); unit != OBC_NULL_INDEX; unit = OBC_ListIterNext(points,unit)){
        printf("Point: %u, %u\n", (*points)[unit].x, (*points)[unit].y);
    }
    //Equivalent
    puts("-------------");
    Point *const pt = *points;
    OBC_ListForEach(unit,points){
        pt[unit].x++;
        printf("Point: %u, %u\n", pt[unit].x, pt[unit].y);
    }
    / ************** /

    OBC_freeTree(points);
    **/
}


int testAllocRay(){

    const unsigned int allocSize = 1<<20;

    Point pgrei;

    pgrei.x = 43;
    pgrei.y = 77;


    size_t i;

    long long int accu = 0;

    Point **datas = (Point**)OBC_newAllocRay(sizeof(Point));

    printf("START\n");
    volatile clock_t t1 = clock();
    size_t prev = 0,cur = 0;
    for(i = 0; i < allocSize; i++){
        cur = OBC_AllocRayMalloc(datas);
        if(cur-prev != 1){
            printf("FAILURE: %u     PREV: %u\n",cur,prev);
        }
        prev = cur;
        if(!(cur&0xFFFF)){
            printf("%u\n",cur);
        }
    }
    volatile clock_t t2 = clock();



    accu=0;
    srand(6464);
    volatile clock_t a1 = clock();
    for(i = 0; i < allocSize; i++){
        size_t dar = rand()%allocSize;
        accu+=datas[obc(dar)].x;
        accu+=datas[obc(dar)].y;
    }
    volatile clock_t a2 = clock();


    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < allocSize; i++){
        size_t dar = rand()%allocSize;
        OBC_AllocRayFree(datas,dar);
        size_t allc = OBC_AllocRayMalloc(datas);
        if(allc!=dar){
            printf("FAIL_RAND ALLOC: %u     DAR: %u\n",allc,dar);
        }
        datas[obc(dar)] = pgrei;
    }
    volatile clock_t t4 = clock();


    accu=0;
    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < allocSize; i++){
        size_t dar = rand()%allocSize;
        accu+=datas[obc(dar)].x;
        accu+=datas[obc(dar)].y;
    }
    volatile clock_t a4 = clock();


    printf("Deallocation\n");
    printf("BackedSize: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->backed.curUnitLength);
    printf("BackedNoFreeMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    volatile clock_t t5 = clock();

    for(i = 0; i < allocSize; i++){
        OBC_AllocRayFree(datas,i);
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    //*
    for(i = 0; i < allocSize; i++){
        size_t allc = OBC_AllocRayMalloc(datas);
        if(allc!=i){
            printf("FAIL_ALL ALLOC: %u     EXPECT: %u\n",allc,i);
        }
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCLIST(datas)->meta.curUnitLength);
    /*/
        OBC_freeAllocFast(datas);
    //*/
    volatile clock_t t6 = clock();

    puts("\n\n");
    printf("AllocCopy Time: %f\n",1000.0*(t2-t1)/CLOCKS_PER_SEC);
    printf("RandomAlloc Time: %f\n",1000.0*(t4-t3)/CLOCKS_PER_SEC);
    printf("Deallocation Time: %f\n",1000.0*(t6-t5)/CLOCKS_PER_SEC);
    printf("InitRead Time: %f\n",1000.0*(a2-a1)/CLOCKS_PER_SEC);
    printf("RandomRead Time: %u\n",a4-a3);


    return 0;

}



int mainX(int argc, char** argv){

    puts("AAA");

    rgei.baz[3] = 42;
    rgei.ka = 666;
    rgei.kl = 777;
    rgei.we34=333;

    //stackExamples();
    //listExamples();
    //listTests();
    queueTests();

    //*
    ///allocfastbitTest();
    //allocfastbit2Test();
    /*/
    allocfastTest();
    //*/

    /*

    volatile clock_t t6 = clock();
    char **data = OBC_newRay(256,sizeof(char));
    while(OBC_RayMaxLength(data) < 500000000){
        volatile clock_t t1 = clock();
        if(OBC_memset(*data,0,OBC_TO_RAY_PTR(data)->maxLength) != *data){
            printf("Failure: \n");
        }
        volatile clock_t t2 = clock();
        printf("Fill Time: %f     for: %u\n",1000.0*(t2-t1)/CLOCKS_PER_SEC, OBC_TO_RAY_PTR(data)->maxLength);
        if(OBC_RayExpand(data)==OBC_ERROR_FAILURE){
            printf("FAILURE EXPAND\n");
            return -1;
        }
    }

    /*/

    //allocfastTest();

    //*/

    //*
        //allocatorStressTest();
    /*/
    lazyStressTest();
    //*/
    return 0;
}

int mainXZ(int argc, char** argv){

    printf("HashmapSize: %u\n\n\n",(size_t)sizeof(OBC_HashMap));

    srand(time(NULL));
    RNG_seed(time(NULL));

    OBC_Hash value = 0;
    /*
    while(value==0)OBC_hashGenerateLikelyPrime();
    printf("Likely Prime: %u\n",value);
    /*/
    ///printf("Likely Prime: %u\n",(size_t)OBC_hashGeneratePrimeMillerRabn(29,4));
    //*/
    return 0;
}

typedef struct Garbo{
    char str[8];
}Garbo;
unsigned int GarboCmpr(Garbo *base, Garbo *cmpr){
    unsigned int i;
    for(i = 0; i < 8; i++){
        if((base->str[i] - cmpr->str[i]) != 0 ){
            return (unsigned int)(base->str[i] - cmpr->str[i]);
        }
    }
    return 0;
}
BIG_DATA zeroBigData;
void testHashMap(){

    srand(time(NULL));
    zeroBigData.dump[0] = rand();


    Point** map = OBC_newHashMap(sizeof(unsigned int),sizeof(Point));

    unsigned int** keys = OBC_HashMapGetKeyPointer(map);

    Point dump;

    OBC_HashMapIterator iter;
    OBC_Hash hash;
    unsigned int i;
    for(i = 0; i < 32; i++){
        dump.x = i;
        dump.y = i;
        printf("POS %i\n",i);
        hash = /**/OBC_hash(&i, sizeof(unsigned int));/*/i;//*/
        OBC_HashMapPutLoop__(map,&iter,&i,hash,&dump){
            iter.keyCmpr = (keys[obc(iter.iter)] == i);
        }

        printf("Hash Loc: %u\n",iter.iter);
    }
    for(i = 0; i < 32; i++){
        dump.x = i;
        dump.y = i;
        hash = /**/OBC_hash(&i, sizeof(unsigned int));/*/i;//*/
        puts("==========");
        OBC_HashMapGetLoop__(map,&iter,hash){
            printf(">>> KEY_READ: %u  :: CHECK: %u\n",keys[obc(iter.iter)], i);
            iter.keyCmpr = (keys[obc(iter.iter)] == i);
            printf(">>> CMPR: %u   :: CHECK= %u\n",iter.keyCmpr, (keys[obc(iter.iter)] == i));
        }
        printf("UNIT: %u\n",i);
        printf("Hash Loc: %u\n",iter.iter);
        printf("Point X: %u\n",map[obc(iter.iter)].x);
        printf("Point Y: %u\n",map[obc(iter.iter)].y);
        printf("DUMP X: %u\n",dump.x);
        printf("DUMP Y: %u\n",dump.y);
    }

    OBC_Hash *hashes = ((OBC_HashMap *)OBC_TO_HASHMAP_PTR(map))->keyHashes.rawData;
    unsigned int counter = 0;
    for(i = 0; i <
    ((OBC_HashMap *)OBC_TO_HASHMAP_PTR(map))->keyHashes.maxUnitLength;i++){

        if(hashes[i]!= OBC_X_HASHMAP_HASH_EMPTY){printf("Key At: %u :: %u\n",i,keys[obc(i)]);
            printf("Hash At: %u :: %u\n",i,hashes[i]);
            printf("         %u ::  X: %u\n",i, map[obc(i)].x);
            printf("         %u ::  Y: %u\n",i, map[obc(i)].y);
            counter++;
        }
    }
    printf("COUNT: %u\n",counter);
    printf("TOTAL: %u\n",((OBC_HashMap *)OBC_TO_HASHMAP_PTR(map))->keyHashes.maxUnitLength);

    OBC_freeHashMap(map);

    puts("\n\nStarting Benchmarking\n\n");

    Point **mmap = OBC_newHashMap(sizeof(unsigned int), sizeof(Point));
    keys = OBC_HashMapGetKeyPointer(mmap);
    OBC_Hash** hhashes = OBC_HashMapGetHashPointer(mmap);

    unsigned int SIZE = 10000000;//10000000;

    clock_t t0 = clock();
    for(i = 0; i < SIZE; i++){
        hash = /**OBC_hash(&i, sizeof(unsigned int));/*/i;//*/
        //zeroBigData.dump[0] = rand();
        OBC_HashMapPutLoop__(mmap,&iter,&i,hash,&dump){
            iter.keyCmpr = (keys[obc(iter.iter)] == i);
            /*if(iter.keyCmpr != 0){
                printf("SAAME %u :: %u\n",i, keys[obc(iter.iter)]);
            }*/
        }
        /*
        if(iter.X_storage == OBC_NULL_INDEX){
            printf("PUT FAIL: %u     CMPR= %u     FROM: %u\n",i,iter.keyCmpr, keys[obc(iter.iter)]);
        }else{
            //printf("%u :: %u / %u\n",i, keys[obc(iter.X_storage)], iter.X_storage);
        }*/
    }
    clock_t t1 = clock();

    puts("Rand Read");

    unsigned int accu,j;
    clock_t t2 = clock();
    for(i = 0; i < SIZE; i++){
        hash = /**OBC_hash(&i, sizeof(unsigned int));/*/i;//*/
        OBC_HashMapGetLoop__(mmap,&iter,hash){
            iter.keyCmpr = (keys[obc(iter.iter)] == i);
        }
        /*if(iter.iter == OBC_NULL_INDEX){
            printf("FAILE: %u   NEED: %u     RAW: %u\n",i,iter.hash, hash);
            OBC_Iterator kiter = OBC_X_HASHMAP_HASH_TO_POSITION(hash,(((OBC_HashMap*)OBC_TO_HASHMAP_PTR(mmap))));
            for(j = 0; j < ((OBC_HashMap*)OBC_TO_HASHMAP_PTR(mmap))->itemsPerBucket; j++){
                printf("HASH: %u\n",hhashes[obc(kiter+j)]);
                printf("KEY: %u\n",keys[obc(kiter+j)]);
            }
        }*/
        //accu+=mmap[obc(iter.iter)].dump[0];
        accu+=mmap[obc(iter.iter)].x;
    }
    clock_t t3 = clock();
printf("RCOUNT: %u\n",accu);
    printf("Raw Put Time: %ums\n",t1-t0);
    printf("Raw GetAdd Time: %ums\n",t3-t2);

    OBC_HashMap *kmap = OBC_TO_HASHMAP_PTR(mmap);
    printf("Bucket Total: %u\n",kmap->buckets);
    printf("Items Per Bucket: %u\n",kmap->itemsPerBucket);
    printf("Total Hash Units: %u\n",kmap->keyHashes.maxUnitLength);
    printf("Total Key Units: %u\n",kmap->keyHashes.maxUnitLength);
    printf("Total Value Units: %u\n",kmap->values.maxUnitLength);
    printf("Total Items Contained: %u\n",kmap->count);


    OBC_freeHashMap(mmap);


    Garbo garbo;
    garbo.str[7] = '\0';

    OBC_HashMap raww;
    OBC_initHashMap(&raww, sizeof(Garbo), sizeof(unsigned int));
    unsigned int **s_data = (unsigned int**)OBC_HashMapGetDataPointer(&raww);
    Garbo **s_keys = (Garbo**)OBC_HashMapGetKeyPointerRaw(&raww);
    OBC_Hash **s_hash_dump = (OBC_Hash**)OBC_HashMapGetHashPointerRaw(&raww);

    size_t KLA_SIZE = 10000000;//1374259;
    Garbo **s_dump = OBC_newRay(sizeof(Garbo));
    RNG_seed(8787);
    RNG_seed(RNG_rand());
    srand(8787);
    for(i = 0; i < KLA_SIZE; i++){
        for(j = 0; j < 7; j++){
            garbo.str[j] = (RNG_rand()%26)+'a';
        }
        /*OBC_Offset next = OBC_RayNewElement(s_dump);
        if(next == OBC_NULL_INDEX){
            puts("STRING FAILURE");
        }
        s_dump[obc(next)] = garbo;*/
        OBC_RayAddValue(s_dump,garbo);
    }

    t0 = clock();
    for(i = 0; i < KLA_SIZE; i++){

        /*if(i == 3786857 || i == 6671693 || i == 8363730){
            printf("unit: %u   stored: %u\n", i, iter.X_storage);
            OBC_HashMapPutIterStart(s_data, &iter);
            OBC_Offset start = iter.iter;
            OBC_Offset end = start+OBC_HashMap_itemsPerBucket(s_data);
            for(;start < end; start++){
                if(s_hash_dump[obc(start)] < OBC_X_HASHMAP_HASH_FREED){
                    printf("N: %u  P: %u    KEY: %s  PUT: %s\n", i, start, &s_keys[obc(start)].str, &s_dump[obc(i)].str);
                }else{
                    printf("N: %u  P: %u    KEY: NONE     PUT: %s\n", i, start, &s_dump[obc(i)].str);
                }
            }
        }*/

        hash = OBC_hash(& s_dump[obc(i)].str, sizeof(garbo.str));
        OBC_HashMapPutLoop__(s_data,&iter,&s_dump[obc(i)],hash,&i){
            ///if(i != 3786857 || i == 6671693 || i == 8363730){
            ///    printf("N: %u   KEY: %s   PUT: %s\n", i, &s_keys[obc(iter.iter)].str, &s_dump[obc(i)].str);
            ///}
            //*
            for(j = 0; j < 8; j++){
                iter.keyCmpr = (s_keys[obc(iter.iter)].str[j] == s_dump[obc(i)].str[j] );
                if(iter.keyCmpr != 1){
                    break;
                }
            }
            /*/
            iter.keyCmpr = GarboCmpr(&s_keys[obc(iter.iter)], &s_dump[obc(i)]) == 0;
            //*/
        }
        if(iter.X_storage == OBC_NULL_INDEX){
            ///printf("FAILED TO ADD TO MAP: %u\n",i);
        }
    }
    t1 = clock();

    t2 = clock();
    for(i = 0; i < KLA_SIZE; i++){
        hash = OBC_hash(& s_dump[obc(i)].str, sizeof(garbo.str));
        OBC_HashMapGetLoop__(s_data,&iter,hash){
            /*
            for(j = 0; j < 7; j++){
                iter.keyCmpr = (s_keys[obc(iter.iter)].str[j] == s_dump[obc(i)].str[j] );
                if(iter.keyCmpr != 1){
                    break;
                }
            }
            /*/
            iter.keyCmpr = GarboCmpr(&s_keys[obc(iter.iter)], &s_dump[obc(i)]) == 0;
            //*/
        }
        if(iter.keyCmpr == 0){
            printf("FAILIED TO FIND:: %u\n",i);
        }
        //accu+=mmap[obc(iter.iter)].x;
    }
    t3 = clock();

    puts("==================");

    printf("Raw Put Time: %ums\n",t1-t0);
    printf("Raw Get Time: %ums\n",t3-t2);

    printf("Bucket Total: %u\n",raww.buckets);
    printf("Items Per Bucket: %u\n",raww.itemsPerBucket);
    printf("Total Hash Units: %u\n",raww.keyHashes.maxUnitLength);
    printf("Total Key Units: %u\n",raww.keyHashes.maxUnitLength);
    printf("Total Value Units: %u\n",raww.values.maxUnitLength);
    printf("Total Items Contained: %u\n",raww.count);

    puts("==================");

    //OBC_freeHashMapData(&raww);
    OBC_freeRay(s_dump);

    Garbo **u_set = OBC_newRay(sizeof(Garbo));

    puts("Attempting to Find Duplications In Keys");
    OBC_HashMapForEach(s_data,&iter){
        OBC_RayAddValue(u_set, s_keys[obc(iter.iter)]);
    }


    printf("Unsorted Ray Size: %u\n", OBC_Ray_CurUnitLength(u_set));
    t0 = clock();
    qsort(*u_set, OBC_Ray_CurUnitLength(u_set), sizeof(Garbo), GarboCmpr);
    t1 = clock();
    printf("Sort Time: %ums\n",t1-t0);

    OBC_RayIterator riter;
    OBC_RayForEachSub(u_set, &riter, 0, 1){
        if(GarboCmpr(&u_set[obc(riter.iter)], &u_set[obc(riter.iter+1)]) == 0){
            printf("DUPLICATE: %u   ITEM: %s   NEXT: %s\n", riter.iter, & u_set[obc(riter.iter)].str, & u_set[obc(riter.iter+1)].str );
            //if(i == 3786857 || i == 6671693 || i == 8363730){
            //printf("unit: %u   stored: %u\n", i, iter.X_storage);

                iter.hash = OBC_HashMapFitHash(s_data, OBC_hash(& garbo.str, sizeof(garbo.str)));
                OBC_HashMapGetIterStart(s_data, &iter);
                OBC_Offset start = iter.iter;
                printf("START_POS: %u\n", iter.iter);
                OBC_Offset end = start+OBC_HashMap_itemsPerBucket(s_data);
                for(;start < end; start++){
                    if(s_hash_dump[obc(start)] < OBC_X_HASHMAP_HASH_FREED){
                        printf("N: %u  P: %u    KEY: %s  PUT: %s\n", iter.hash, start, &s_keys[obc(start)].str, &s_dump[obc(i)].str);
                    }else{
                        printf("N: %u  P: %u    KEY: NONE     PUT: %s\n", iter.hash, start, &s_dump[obc(i)].str);
                    }
                }
            //}
        }
    }

    OBC_freeHashMapData(&raww);
    OBC_freeRay(u_set);
}

int main(int argc, char** argv){

    testHashMap();
    //listTests();

    printf("AllocSize: %u\n",sizeof(OBC_AllocListBit));
    printf("int size: %u  :: OFFSET SIZE: %u\n",(unsigned int)sizeof(unsigned int),(unsigned int)sizeof(OBC_Offset));

    srand(time(NULL));

    puts("init\n");
    Point** stuff = (Point**)OBC_newAllocListBit(sizeof(Point));
    puts("done init\n");

    const unsigned int MAX = 1<<23;
    unsigned int i;
    size_t pos;

    pos = OBC_AllocListBitMalloc(stuff);
    if(pos != 0){
        exit(-5);
    }
    pos = OBC_AllocListBitMalloc(stuff);
    if(pos != 1){
        exit(-6);
    }
    pos = OBC_AllocListBitMalloc(stuff);
    if(pos != 2){
        exit(-7);
    }
    pos = OBC_AllocListBitMalloc(stuff);
    if(pos != 3){
        exit(-8);
    }
    OBC_AllocListBitFree(stuff,1);
    pos = OBC_AllocListBitMalloc(stuff);
    if(pos != 1){
        exit(-9);
    }
    OBC_AllocListBitFree(stuff,0);
    OBC_AllocListBitFree(stuff,2);
    OBC_AllocListBitFree(stuff,1);
    OBC_AllocListBitFree(stuff,3);

    clock_t start,end;
    const unsigned int ROUNDS = 1;

    unsigned long long int diff = 0;
    unsigned int k;
    puts("starting cache alloc\n");
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        for(i = 0; i < MAX; i++){
            pos = OBC_AllocListBitMalloc(stuff);
            if(pos != 0){
                puts("FAILURE CACHE ALLOC");
                exit(-43);
            }
            OBC_AllocListBitFree(stuff,0);
        }
        end = clock();
        printf("Cache Malloc/Free TIME:%u ms\n",(unsigned int)((end-start)*1000/CLOCKS_PER_SEC));
        diff += end-start;
    }
    printf("Cache Malloc/Free AVERAGE TIME:%u ms\n",(unsigned int)(diff/ROUNDS));

    diff = 0;
    puts("starting 16mil alloc\n");
    for(k = 0; k < ROUNDS; k++){
        OBC_freeAllocListBit(stuff);
        stuff = (Point**)OBC_newAllocListBit(sizeof(Point));
        start = clock();
        for(i = 0; i < MAX; i++){
            pos = OBC_AllocListBitMalloc(stuff);
            if(pos != i){
                printf("POS:%i != %i\n", (unsigned int)pos,i);
                exit(-1);
            }
        }
        end = clock();
        printf("16Mil Alloc TIME:%u ms\n",(unsigned int)((end-start)*1000/CLOCKS_PER_SEC));
        diff += end-start;
    }
    printf("16Mil Alloc AVERAGE TIME:%u ms\n",(unsigned int)(diff/ROUNDS));


    unsigned int kpos = pos;
    OBC_AllocListBitFree(stuff,kpos);
    puts("starting post cache alloc\n");
    diff = 0;
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        for(i = 0; i < MAX; i++){
            pos = OBC_AllocListBitMalloc(stuff);
            if(pos != kpos){
                puts("FAILURE ALLOC");
                exit(-43);
            }
            OBC_AllocListBitFree(stuff,kpos);
        }
        end = clock();
        printf("Post Cache alloc TIME:%u ms\n",(unsigned int)((end-start)*1000/CLOCKS_PER_SEC));
        diff += end-start;
    }
    printf("Post Cache alloc AVERAGE TIME:%u ms\n",(unsigned int)(diff/ROUNDS));

    printf("PRE REALLOCD: %u\n",OBC_AllocListBitMalloc(stuff));
    OBC_AllocListBitFree(stuff,18);
    OBC_AllocListBitFree(stuff,98076);
    OBC_AllocListBitFree(stuff,865);
    OBC_AllocListBitFree(stuff,5685684);
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",(unsigned int)OBC_AllocListBitMalloc(stuff));
    //**/


    puts("starting random cache free and alloc\n");
    diff = 0;
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        //*
        for(i = 0; i < MAX; i++){

            kpos =
                (((unsigned int)0)^rand())&(MAX-1);
            OBC_AllocListBitFree(stuff,kpos);
            pos = OBC_AllocListBitMalloc(stuff);

            if(pos != kpos){
                puts("FAILURE RAND ALLOC");
                exit(-43);
            }

        }
        end = clock();
        printf("Post Random alloc TIME:%u ms\n",(unsigned int)((end-start)*1000/CLOCKS_PER_SEC));
        diff += end-start;
    }
    printf("Post Random alloc AVERAGE TIME:%u ms\n",(unsigned int)(diff/ROUNDS));

    OBC_AllocListBitFree(stuff,0);
    printf("ALLOCD: %u\n",OBC_AllocListBitMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocListBitMalloc(stuff));

    OBC_freeAllocListBit(stuff);

    return 0;
}

int mainQUEUE(int argc, char** argv){

    Point** stuff = (Point**)OBC_newQueue(sizeof(Point));

    Point dumb;
    dumb.x = 0; dumb.y = 1;
    if(OBC_QueueAdd(stuff,&dumb) == OBC_ERROR_FAILURE){
        exit(-1);
    }

    printf("%u , %u\n",stuff[obc(0)].x,stuff[obc(0)].y);

    OBC_freeQueue(stuff);

    puts("\n\n");
    printf("Hashmap Size: %u\n",sizeof(OBC_HashMap));

    //stackExamples();
    //listTests();
    //queueTests();

    return 0;
}

int mainK(int argc, char** argv){

    const int size = 99000000*sizeof(int);
    const int len = size/4;

    srand(time(NULL));

    int *data = (int *)malloc(size);
    int *copy = (int *)malloc(size);
    if(data == NULL || copy == NULL){
        exit(-88);
    }

    //memset(data,0,size);

    const size_t ROUNDS = 100;

    double clocks = 0.0;
    size_t round;


    volatile clock_t ss = clock();
    int i;
    for(i = 0; i < len; i++){
        data[i] = 43;
        copy[i] = 88;
    }
    volatile clock_t dd = clock();
    //memset(copy,42,size);
    printf("Clocks For fill Data: %ums\n",(unsigned int)((dd-ss)*1000/CLOCKS_PER_SEC/ROUNDS));


    printf("Size of Swapping %i\n",sizeof(OBC_LONGEST));





    clocks = 0.0;
    for(round = 0; round < ROUNDS; round++){

        volatile clock_t start = clock();

        memswap3(copy,data,size);

        volatile clock_t end = clock();
        clocks+=(end-start);
    }
    printf("Total Clocks For memswap3: %f\n",clocks);
    printf("Clocks For memswap3: %ums\n",(unsigned int)(clocks*1000/CLOCKS_PER_SEC/ROUNDS));


    clocks = 0.0;
    for(round = 0; round < ROUNDS; round++){

        volatile clock_t start = clock();

        memswap2(copy,data,size);

        volatile clock_t end = clock();
        clocks+=(end-start);
    }
    printf("Total Clocks For memswap2: %f\n",clocks);
    printf("Clocks For memswap2: %ums\n",(unsigned int)(clocks*1000/CLOCKS_PER_SEC/ROUNDS));



    clocks = 0.0;
    for(round = 0; round < ROUNDS; round++){

        volatile clock_t start = clock();

        memswap1(copy,data,size);

        volatile clock_t end = clock();
        clocks+=(end-start);
    }
    printf("Total Clocks For memswap1: %f\n",clocks);
    printf("Clocks For memswap1: %ums\n",(unsigned int)(clocks*1000/CLOCKS_PER_SEC/ROUNDS));


    clocks = 0.0;
    for(round = 0; round < ROUNDS; round++){

        volatile clock_t start = clock();

        memswap0(copy,data,size);
        //printf("DATA: %i   COPY: %i\n",data[34],copy[34]);

        volatile clock_t end = clock();
        clocks+=(end-start);
    }
    printf("Total Clocks For memswap0: %f\n",clocks);
    printf("Clocks For memswap0: %ums\n",(unsigned int)(clocks*1000/CLOCKS_PER_SEC/ROUNDS));



    clocks = 0.0;
    for(round = 0; round < ROUNDS; round++){

        volatile clock_t start = clock();

        memswap_inline(copy,data,size);
        //printf("DATA: %i   COPY: %i\n",data[34],copy[34]);

        volatile clock_t end = clock();
        clocks+=(end-start);
    }
    printf("Total Clocks For INLINEmemswap0: %f\n",clocks);
    printf("Clocks For INLINEmemswap0: %ums\n",(unsigned int)(clocks*1000/CLOCKS_PER_SEC/ROUNDS));

    return 0;

}


int mainXX(int argc, char** argv){

    void *ptr = NULL;
        ptr++;
    int value = 0;
        value++;

    int i;
    for(i = 0; i < sizeof(ptr); i++){
        printf("Pointer: %i\n",(int)(((unsigned char*)&ptr)[i]));
    }
    for(i = 0; i < sizeof(value); i++){
        printf("Integer: %i\n",(int)(((unsigned char*)&value)[i]));
    }

    return 0;
}
