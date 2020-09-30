#include "memswap.h"


void memswap0(void* __restrict b, void* __restrict a, size_t sizeInBytes)
{

	size_t i = 0;
	if(sizeInBytes >= sizeof(OBC_LONGEST)*4){
		do{
            sizeInBytes-=sizeof(OBC_LONGEST)*4;

			OBC_LONGEST tmp0 = ((OBC_LONGEST *)a)[i];
			((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];
			((OBC_LONGEST *)b)[i] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+1];
			((OBC_LONGEST *)a)[i+1] = ((OBC_LONGEST *)b)[i+1];
			((OBC_LONGEST *)b)[i+1] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+2];
			((OBC_LONGEST *)a)[i+2] = ((OBC_LONGEST *)b)[i+2];
			((OBC_LONGEST *)b)[i+2] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+3];
			((OBC_LONGEST *)a)[i+3] = ((OBC_LONGEST *)b)[i+3];
			((OBC_LONGEST *)b)[i+3] = tmp0;

			i+=4;
		}while(sizeInBytes > sizeof(OBC_LONGEST)*4);
	}

	if(sizeInBytes >= sizeof(OBC_LONGEST)){
		do{
            sizeInBytes-=sizeof(OBC_LONGEST);
			OBC_LONGEST tmp0 = ((OBC_LONGEST *)a)[i];
			((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];
			((OBC_LONGEST *)b)[i] = tmp0;

			i+=1;
		}while(sizeInBytes > sizeof(OBC_LONGEST));
	}

	i*=sizeof(OBC_LONGEST);

	if(sizeInBytes){
        do{
            sizeInBytes--;
            unsigned char tmp0 = ((unsigned char *)a)[i];
            ((unsigned char *)a)[i] = ((unsigned char *)b)[i];
            ((unsigned char *)b)[i] = tmp0;
            i++;
        }while(sizeInBytes);
	}

}

void memswap1(void* __restrict b, void* __restrict a, size_t sizeInBytes)
{

	size_t i = 0;
	if(sizeInBytes >= sizeof(OBC_LONGEST)*4){
        sizeInBytes-=sizeof(OBC_LONGEST)*4;
		do{

			OBC_LONGEST tmp0 = ((OBC_LONGEST *)a)[i];
			((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];
			((OBC_LONGEST *)b)[i] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+1];
			((OBC_LONGEST *)a)[i+1] = ((OBC_LONGEST *)b)[i+1];
			((OBC_LONGEST *)b)[i+1] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+2];
			((OBC_LONGEST *)a)[i+2] = ((OBC_LONGEST *)b)[i+2];
			((OBC_LONGEST *)b)[i+2] = tmp0;
			tmp0 = ((OBC_LONGEST *)a)[i+3];
			((OBC_LONGEST *)a)[i+3] = ((OBC_LONGEST *)b)[i+3];
			((OBC_LONGEST *)b)[i+3] = tmp0;

			sizeInBytes-=sizeof(OBC_LONGEST)*4;
			i+=4;
		}while(sizeInBytes > sizeof(OBC_LONGEST)*4);
		i*=sizeof(OBC_LONGEST);
	}

	if(sizeInBytes){
        while(sizeInBytes){
            unsigned char tmp = ((unsigned char *)a)[i];
            ((unsigned char *)a)[i] = ((unsigned char *)b)[i];
            ((unsigned char *)b)[i] = tmp;
            i++;
            sizeInBytes--;
        }
	}
}





void memswap2(void * __restrict ptr0, void * __restrict ptr1, size_t sizeInBytes){

    union tmp{
        OBC_LONGEST tmp0;
        unsigned int tmp1;
        unsigned char tmp2;
    }tmp;


	if(sizeInBytes >= sizeof(OBC_LONGEST)){
		do{
            sizeInBytes-=sizeof(OBC_LONGEST);

			tmp.tmp0 = *((OBC_LONGEST*)((ptr0+sizeInBytes)));
			*((OBC_LONGEST*)((ptr0+sizeInBytes))) = *((OBC_LONGEST*)((ptr1+sizeInBytes)));
            *((OBC_LONGEST*)((ptr1+sizeInBytes))) = tmp.tmp0;

		}while(sizeInBytes > sizeof(OBC_LONGEST));
	}

    if(sizeInBytes >= sizeof(unsigned int)){
        do{
            sizeInBytes-=sizeof(unsigned int);

            tmp.tmp1 = *((unsigned int*)((ptr0+sizeInBytes)));
			*((unsigned int*)((ptr0+sizeInBytes))) = *((unsigned int*)((ptr1+sizeInBytes)));
            *((unsigned int*)((ptr1+sizeInBytes))) = tmp.tmp1;

        }while(sizeInBytes > sizeof(unsigned int));
    }

    if(sizeInBytes){
        do{
            sizeInBytes-=sizeof(unsigned char);

            tmp.tmp2 = *((unsigned char*)((ptr0+sizeInBytes)));
			*((unsigned char*)((ptr0+sizeInBytes))) = *((unsigned char*)((ptr1+sizeInBytes)));
            *((unsigned char*)((ptr1+sizeInBytes))) = tmp.tmp2;

        }while(sizeInBytes);
    }

}

void memswap3(void * __restrict a, void * __restrict b, size_t sizeInBytes)
{
	OBC_LONGEST tmp0;
	unsigned char tmp1;

	size_t i = 0;
	if(sizeInBytes > sizeof(OBC_LONGEST)){
		do{

			tmp0 = ((OBC_LONGEST *)a)[i];
			((OBC_LONGEST *)a)[i] = ((OBC_LONGEST *)b)[i];
			((OBC_LONGEST *)b)[i] = tmp0;

			sizeInBytes-=sizeof(OBC_LONGEST);
			i+=1;
		}while(sizeInBytes > sizeof(OBC_LONGEST));
		i*=sizeof(OBC_LONGEST);
	}

	while(sizeInBytes){
        tmp1 = ((unsigned char *)a)[i];
			((unsigned char *)a)[i] = ((unsigned char *)b)[i];
			((unsigned char *)b)[i] = tmp1;
		i++;
        sizeInBytes--;
	}
}



