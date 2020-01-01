#include "memswap.h"

void *OBC_memswap(void * __restrict ptr0, void * __restrict ptr1, size_t sizeInBytes){

    union tmp{
        X_X_LONGEST tmp0;
        unsigned int tmp1;
        unsigned char tmp2;
    }tmp;


	if(sizeInBytes >= sizeof(X_X_LONGEST)){
		do{
            sizeInBytes-=sizeof(X_X_LONGEST);

			tmp.tmp0 = *((X_X_LONGEST*)((ptr0+sizeInBytes)));
			*((X_X_LONGEST*)((ptr0+sizeInBytes))) = *((X_X_LONGEST*)((ptr1+sizeInBytes)));
            *((X_X_LONGEST*)((ptr1+sizeInBytes))) = tmp.tmp0;

		}while(sizeInBytes > sizeof(X_X_LONGEST));
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

    return ptr0;
}

void OBC_swapX(void * __restrict a, void * __restrict b, size_t sizeInBytes)
{
	X_X_LONGEST tmp0;
	unsigned char tmp1;

	size_t i = 0;
	if(sizeInBytes > sizeof(X_X_LONGEST)){
		do{

			tmp0 = ((X_X_LONGEST *)a)[i];
			((X_X_LONGEST *)a)[i] = ((X_X_LONGEST *)b)[i];
			((X_X_LONGEST *)b)[i] = tmp0;

			sizeInBytes-=sizeof(X_X_LONGEST);
			i+=1;
		}while(sizeInBytes > sizeof(X_X_LONGEST));
		i*=sizeof(X_X_LONGEST);
	}

	while(sizeInBytes){
        tmp1 = ((unsigned char *)a)[i];
			((unsigned char *)a)[i] = ((unsigned char *)b)[i];
			((unsigned char *)b)[i] = tmp1;
		i++;
        sizeInBytes--;
	}
}

void memswap2(X_X_LONGEST* __restrict b, X_X_LONGEST* __restrict a, size_t sizeInBytes)
{

	size_t i = 0;
	if(sizeInBytes >= sizeof(X_X_LONGEST)*4){
        sizeInBytes-=sizeof(X_X_LONGEST)*4;
		do{

			X_X_LONGEST tmp0 = a[i];
			a[i] = b[i];
			b[i] = tmp0;
			tmp0 = a[i+1];
			a[i+1] = b[i+1];
			b[i+1] = tmp0;
			tmp0 = a[i+2];
			a[i+2] = b[i+2];
			b[i+2] = tmp0;
			tmp0 = a[i+3];
			a[i+3] = b[i+3];
			b[i+3] = tmp0;

			sizeInBytes-=sizeof(X_X_LONGEST)*4;
			i+=4;
		}while(sizeInBytes > sizeof(X_X_LONGEST)*4);
		i*=sizeof(X_X_LONGEST);
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



void memswap3(X_X_LONGEST* __restrict b, X_X_LONGEST* __restrict a, size_t sizeInBytes)
{

	size_t i = 0;
	if(sizeInBytes >= sizeof(X_X_LONGEST)*4){
		do{
            sizeInBytes-=sizeof(X_X_LONGEST)*4;

			X_X_LONGEST tmp0 = a[i];
			a[i] = b[i];
			b[i] = tmp0;
			tmp0 = a[i+1];
			a[i+1] = b[i+1];
			b[i+1] = tmp0;
			tmp0 = a[i+2];
			a[i+2] = b[i+2];
			b[i+2] = tmp0;
			tmp0 = a[i+3];
			a[i+3] = b[i+3];
			b[i+3] = tmp0;

			i+=4;
		}while(sizeInBytes > sizeof(X_X_LONGEST)*4);
	}

	if(sizeInBytes >= sizeof(X_X_LONGEST)){
		do{
            sizeInBytes-=sizeof(X_X_LONGEST);
			X_X_LONGEST tmp0 = a[i];
			a[i] = b[i];
			b[i] = tmp0;

			i+=1;
		}while(sizeInBytes > sizeof(X_X_LONGEST));
	}

	i*=sizeof(X_X_LONGEST);

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

