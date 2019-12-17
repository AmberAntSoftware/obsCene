#include "OBC.h"

#define OBC_MEMSET_TYPE unsigned int

void *OBC_memset(void *ptr, int c, size_t len){

    unsigned int i;

    if(len < sizeof(OBC_MEMSET_TYPE)){

        for(i = 0; i < len; i++){
            *((unsigned char*)(ptr+i)) = (unsigned char)c;
        }

        return ptr;
    }

    //*
    OBC_MEMSET_TYPE fill = 0;
    for(i = 0; i < sizeof(OBC_MEMSET_TYPE); i++){
        fill<<=CHAR_BIT;
        fill|=(OBC_MEMSET_TYPE)c;
    }

    if(len > sizeof(OBC_MEMSET_TYPE)*8){
        do{
            len-=sizeof(OBC_MEMSET_TYPE)*8;
            *((((OBC_MEMSET_TYPE*)(ptr+len)))) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+1)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+2)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+3)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+4)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+5)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+6)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+7)) = fill;
        }while(len > sizeof(OBC_MEMSET_TYPE)*8);
    }

    if(len > sizeof(OBC_MEMSET_TYPE)){
        do{
            len-=sizeof(OBC_MEMSET_TYPE);
            *((((OBC_MEMSET_TYPE*)(ptr+len)))) = fill;
        }while(len > sizeof(OBC_MEMSET_TYPE));
    }

    if(len > 0){
        do{
            len--;
            *((unsigned char*)(ptr+i)) = (unsigned char)c;
        }while(len > 0);
    }

    return ptr;

}

/*
void *OBC_memcpy(void *dst, const void * const src, size_t len){

    unsigned int i;

    if(len < sizeof(OBC_MEMSET_TYPE)){

        for(i = 0; i < len; i++){
            *((unsigned char*)(ptr+i)) = (unsigned char)c;
        }

        return ptr;
    }

    //*
    OBC_MEMSET_TYPE fill = 0;
    for(i = 0; i < sizeof(OBC_MEMSET_TYPE); i++){
        fill<<=CHAR_BIT;
        fill|=(OBC_MEMSET_TYPE)c;
    }

    if(len > sizeof(OBC_MEMSET_TYPE)*8){
        do{
            len-=sizeof(OBC_MEMSET_TYPE)*8;
            *((((OBC_MEMSET_TYPE*)(ptr+len)))) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+1)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+2)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+3)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+4)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+5)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+6)) = fill;
            *((((OBC_MEMSET_TYPE*)(ptr+len))+7)) = fill;
        }while(len > sizeof(OBC_MEMSET_TYPE)*8);
    }

    if(len > sizeof(OBC_MEMSET_TYPE)){
        do{
            len-=sizeof(OBC_MEMSET_TYPE);
            *((((OBC_MEMSET_TYPE*)(ptr+len)))) = fill;
        }while(len > sizeof(OBC_MEMSET_TYPE));
    }

    if(len > 0){
        do{
            len--;
            *((unsigned char*)(ptr+i)) = (unsigned char)c;
        }while(len > 0);
    }

    return ptr;

}
*/

unsigned int OBC_timerMillis(const OBC_Timer start, const OBC_Timer end){
    return (end-start)*1000/CLOCKS_PER_SEC;
}

unsigned int OBC_timerSeconds(const OBC_Timer start, const OBC_Timer end){
    return (end-start)/CLOCKS_PER_SEC;
}
