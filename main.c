#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obc_stdlib/memswap.h"

#include "obc.h"

#include "containers/ray.h"
#include "allocators/allocator.h"
#include "allocators/allocfast.h"
#include "allocators/allocfastbitcache.h"
#include "containers/list.h"
#include "containers/stack.h"
#include "containers/queue.h"
#include "containers/tree.h"
#include "containers/hashmap.h"
#include "allocators/allocray.h"

#include "obc_stdlib/hash.h"
#include "obc_stdlib/random.h"


const size_t ALLOC_SIZE = 10000000;

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
    datas = (A**)OBC_newAllocator2(sizeof(A));
    OBC_Allocator2 *dump = OBC_TO_RAW_ALLOCATOR2(datas);
    //datas = OBC_newRay(8,sizeof(A));
    prev = 0;
    size_t i;
    printf("START\n");
    volatile clock_t t1 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        //**
            //printf("%i\n",i);
        cur = OBC_Allocator2Malloc3(datas);
        OBC_Allocator2GetValue(datas,cur)=rgei;
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
        accu+=OBC_Allocator2GetPointer(datas,dar)->we34;
        accu+=OBC_Allocator2GetPointer(datas,dar)->baz[3];
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
    cur = OBC_Allocator2Malloc3(datas);
    printf("GOT: %llu\n",(long long unsigned int)cur);
    cur = OBC_Allocator2Malloc3(datas);
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
    cur = OBC_Allocator2Malloc3(datas);
    printf("CUR: %u\n",cur);
    cur = OBC_Allocator2Malloc3(datas);
    printf("CUR: %u\n",cur);


    printf("\n\n\nSTRESS_TESTING\n\n\n");
    ///srand(time(NULL));
    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < dump->backed.curUnitLength; i++){
        size_t dar = rand()%dump->backed.curUnitLength;
        OBC_Allocator2Free3(datas,dar);
        if(OBC_Allocator2Malloc3(datas) != dar){
            printf("FAIL: %u",dar);
        }
    }
    volatile clock_t t4 = clock();


    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_Allocator2GetPointer(datas,dar)->we34;
        accu+=OBC_Allocator2GetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a4 = clock();

    printf("DeAllocating..\n");
    toTest **stores = malloc(ALLOC_SIZE*sizeof(toTest*));
    for(i=0; i < ALLOC_SIZE; i++)
        stores[i] = NULL;
    volatile clock_t t5 = clock();
    OBC_freeAllocator2(datas);
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

    return 0;
}

int allocfastTest(){

    size_t i;

    long long int accu = 0;

    int size = ALLOC_SIZE;
    size_t *stores = malloc(ALLOC_SIZE*sizeof(size_t));
    toTest **datas = (toTest**)OBC_newAllocFast(sizeof(toTest));


    //*
    printf("START\n");
    volatile clock_t t1 = clock();
    size_t prev = 0,cur = 0;
    for(i = 0; i < ALLOC_SIZE; i++){
        cur = OBC_AllocFastMalloc(datas);
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
        accu+=OBC_AllocFastGetPointer(datas,dar)->we34;
        accu+=OBC_AllocFastGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a2 = clock();


    srand(6464);
    volatile clock_t t3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        OBC_AllocFastFree(datas,dar);
        size_t allc = OBC_AllocFastMalloc(datas);
        if(allc!=dar){
            printf("FAIL_RAND ALLOC: %u     DAR: %u\n",allc,dar);
        }
        OBC_AllocFastGetValue(datas,dar) = rgei;
    }
    volatile clock_t t4 = clock();


    accu=0;
    srand(6464);
    volatile clock_t a3 = clock();
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t dar = rand()%ALLOC_SIZE;
        accu+=OBC_AllocFastGetPointer(datas,dar)->we34;
        accu+=OBC_AllocFastGetPointer(datas,dar)->baz[3];
    }
    volatile clock_t a4 = clock();


    printf("Deallocation\n");
    printf("BackedSize: %u\n",OBC_TO_RAW_ALLOCFAST(datas)->backed.curUnitLength);
    printf("BackedNoFreeMeta: %u\n",OBC_TO_RAW_ALLOCFAST(datas)->meta.curUnitLength);
    volatile clock_t t5 = clock();

    for(i = 0; i < ALLOC_SIZE; i++){
        OBC_AllocFastFree(datas,i);
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCFAST(datas)->meta.curUnitLength);
    //*
    for(i = 0; i < ALLOC_SIZE; i++){
        size_t allc = OBC_AllocFastMalloc(datas);
        if(allc!=ALLOC_SIZE-i){
            printf("FAIL_ALL ALLOC: %u     EXPECT: %u\n",allc,i);
        }
    }
    printf("BackedMeta: %u\n",OBC_TO_RAW_ALLOCFAST(datas)->meta.curUnitLength);
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

    OBC_AllocFast *fast = OBC_TO_RAW_ALLOCFAST(datas);

    printf("BackedDataSize: %u\n",OBC_RayCurByteLength(OBC_FROM_RAY_VAL(fast->backed)));
    printf("BackedMetaSize: %u\n",OBC_RayCurByteLength(OBC_FROM_RAY_VAL(fast->meta)));
    printf("BackedTotalSize: %u\n",OBC_RayCurByteLength(OBC_FROM_RAY_VAL(fast->meta))+OBC_RayCurByteLength(OBC_FROM_RAY_VAL(fast->backed)));


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
        OBC_GetPointer(strs,item)->data = "stuff";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "things";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "forks";
    item = OBC_ListNewItem(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "wkebgkwgek";

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
        OBC_GetPointer(strs,item)->data = "stuff";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "things";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "forks";
    item = OBC_StackPush(strs);
    printf("ITEM: %u\n",item);
        OBC_GetPointer(strs,item)->data = "wkebgkwgek";

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
        OBC_GetPointer(strs,item)->length = 15;
        OBC_GetPointer(strs,item)->data = "wkebgkwgek";
        OBC_GetPointer(strs,item)->small[OBC_GetPointer(strs,item)->length] = 'k';
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
















typedef struct Point{
    size_t x;
    size_t y;
}Point;

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

#include "allocators/allocfastbitcache.h"
int main(int argc, char** argv){

    listTests();
    //allocfastTest();
    //allocfastbitTest();
    //allocfastbit2Test();

    printf("AllocSize: %u\n",sizeof(OBC_AllocFastBitCache));

    srand(time(NULL));

    puts("init\n");
    Point** stuff = (Point**)OBC_newAllocFastBitCache(sizeof(Point));
    puts("done init\n");

    const unsigned int MAX = 1<<23; //10000000;
    unsigned int i;
    size_t pos;

    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos != 0){
        exit(-5);
    }
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos != 1){
        exit(-6);
    }
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos != 2){
        exit(-7);
    }
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos != 3){
        exit(-8);
    }
    OBC_AllocFastBitCacheFree(stuff,1);
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos != 1){
        exit(-9);
    }
    OBC_AllocFastBitCacheFree(stuff,0);
    OBC_AllocFastBitCacheFree(stuff,2);
    OBC_AllocFastBitCacheFree(stuff,1);
    OBC_AllocFastBitCacheFree(stuff,3);

    clock_t start,end;
    const unsigned int ROUNDS = 1;

    unsigned long long int diff = 0;
    unsigned int k;
    puts("starting cache alloc\n");
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        for(i = 0; i < MAX; i++){
            pos = OBC_AllocFastBitCacheMalloc(stuff);
            if(pos != 0){
                puts("FAILURE ALLOC");
                exit(-43);
            }
            OBC_AllocFastBitCacheFree(stuff,0);
        }
        end = clock();
        printf("Cache Malloc/Free TIME:%u ms\n",end-start);
        diff += end-start;
    }
    printf("Cache Malloc/Free AVERAGE TIME:%u ms\n",(diff)/ROUNDS);

    diff = 0;
    puts("starting 16mil alloc\n");
    for(k = 0; k < ROUNDS; k++){
        OBC_freeAllocFastBitCache(stuff);
        stuff = (Point**)OBC_newAllocFastBitCache(sizeof(Point));
        start = clock();
        for(i = 0; i < MAX; i++){
            //puts("Mallocing\n");
            pos = OBC_AllocFastBitCacheMalloc(stuff);
            //puts("Done mallocing\n");
            //printf("I: %u\n", i);
            if(pos != i){
                printf("POS:%i != %i\n", pos,i);
                exit(-1);
            }
        }
        end = clock();
        printf("16Mil Alloc TIME:%u ms\n",end-start);
        diff += end-start;
    }
    printf("16Mil Alloc AVERAGE TIME:%u ms\n",(diff)/ROUNDS);


    unsigned int kpos = pos;
    OBC_AllocFastBitCacheFree(stuff,kpos);
    puts("starting post cache alloc\n");
    diff = 0;
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        for(i = 0; i < MAX; i++){
            pos = OBC_AllocFastBitCacheMalloc(stuff);
            if(pos != kpos){
                puts("FAILURE ALLOC");
                exit(-43);
            }
            OBC_AllocFastBitCacheFree(stuff,kpos);
        }
        end = clock();
        printf("Post Cache alloc TIME:%u ms\n",end-start);
        diff += end-start;
    }
    printf("Post Cache alloc AVERAGE TIME:%u ms\n",(diff)/ROUNDS);

    ///printf("PRE REALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));

    /*
    OBC_AllocFastBitCacheFree(stuff,456);
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos!=456){
        printf("FAILED ALLOCATION: NEED %u ::GOT %u\n",456,pos);
        exit(-767);
    }
    OBC_AllocFastBitCacheFree(stuff,456);
    pos = OBC_AllocFastBitCacheMalloc(stuff);
    if(pos!=456){
        printf("FAILED ALLOCATION: NEED %u ::GOT %u\n",456,pos);
        exit(-767);
    }
    //*/

    ///realloc the other freed test data
    printf("PRE REALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    //printf("PRE REALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    //printf("PRE REALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));

    //**
    OBC_AllocFastBitCacheFree(stuff,18);
    OBC_AllocFastBitCacheFree(stuff,98076);
    OBC_AllocFastBitCacheFree(stuff,865);
    OBC_AllocFastBitCacheFree(stuff,5685684);
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    //**/


    puts("starting random cache free and alloc\n");
    diff = 0;
    for(k = 0; k < ROUNDS; k++){
        start = clock();
        //*
        for(i = 0; i < MAX; i++){

            kpos =
                //rand()&63;
                //(((unsigned int)0)^rand())&63;
                (((unsigned int)0)^rand())&(MAX-1);
            //printf("RandFree: %u\n",kpos);
            OBC_AllocFastBitCacheFree(stuff,kpos);
            pos = OBC_AllocFastBitCacheMalloc(stuff);

            if(pos != kpos){
                puts("FAILURE RAND ALLOC");
                exit(-43);
            }

        }
        /*/
        for(i = MAX-1; i < MAX; i--){
            OBC_AllocFastBitCacheFree(stuff,i);
        }
        for(i = 0; i < MAX; i++){
            kpos = OBC_AllocFastBitCacheMalloc(stuff);
            //printf("%u\n",kpos);
            if( i != kpos){
                printf("NEED: %u  :: GOT: %u\n",i,kpos);
                //puts("FAILURE RE ALLOC");
                //exit(-43);
            }
            /**
            if( ((MAX-i))-32+(i&31)*2 != kpos){
                printf("NEED: %u  :: GOT: %u\n",((MAX-i))-32+(i&31)*2,kpos);
                //puts("FAILURE RE ALLOC");
                //exit(-43);
            }
            //**/
            //if( ((MAX-i))-30 != kpos){
            //    printf("NEED: %u  :: GOT: %u\n",((MAX-i))-30,kpos);
            //    puts("FAILURE RE ALLOC");
            //    exit(-43);
            //}
        //}
        end = clock();
        printf("Post Random alloc TIME:%u ms\n",end-start);
        diff += end-start;
    }
    printf("Post Random alloc AVERAGE TIME:%u ms\n",(diff)/ROUNDS);

    OBC_AllocFastBitCacheFree(stuff,0);
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));
    printf("ALLOCD: %u\n",OBC_AllocFastBitCacheMalloc(stuff));

    OBC_freeAllocFastBitCache(stuff);

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
    allocfastbit2Test();
    allocfastbitTest();

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
