#include "memswap.h"


//https://codereview.stackexchange.com/questions/5520/copying-80-bytes-as-fast-as-possible

void *OBC_memswap(void * __restrict ptr0, void * __restrict ptr1, size_t sizeInBytes){

	X_X_LONGEST tmp0, tmp1;
	unsigned int tmp00, tmp11;
	unsigned char tmp000, tmp111;

	if(sizeInBytes >= sizeof(X_X_LONGEST)){
		do{
            sizeInBytes-=sizeof(X_X_LONGEST);

			tmp0 = *((X_X_LONGEST*)((ptr0+sizeInBytes)));
			tmp1 = *((X_X_LONGEST*)((ptr1+sizeInBytes)));

            *((X_X_LONGEST*)((ptr0+sizeInBytes))) = tmp1;
            *((X_X_LONGEST*)((ptr1+sizeInBytes))) = tmp0;

		}while(sizeInBytes > sizeof(X_X_LONGEST));
	}

    if(sizeInBytes >= sizeof(unsigned int)){
        do{
            sizeInBytes-=sizeof(unsigned int);

            tmp00 = *((unsigned int*)((ptr0+sizeInBytes)));
			tmp11 = *((unsigned int*)((ptr1+sizeInBytes)));

            *((unsigned int*)((ptr0+sizeInBytes))) = tmp11;
            *((unsigned int*)((ptr1+sizeInBytes))) = tmp00;

        }while(sizeInBytes > sizeof(unsigned int));
    }

    if(sizeInBytes){
        sizeInBytes-=sizeof(unsigned char);
        do{
            sizeInBytes-=sizeof(unsigned char);

            tmp000 = *((unsigned char*)((ptr0+sizeInBytes)));
			tmp111 = *((unsigned char*)((ptr1+sizeInBytes)));

            *((unsigned char*)((ptr0+sizeInBytes))) = tmp111;
            *((unsigned char*)((ptr0+sizeInBytes))) = tmp000;

        }while(sizeInBytes);
    }

    return ptr0;
}

void OBC_swap(X_X_LONGEST * __restrict a, X_X_LONGEST* __restrict b, size_t sizeInBytes)
{
	X_X_LONGEST tmp0;
	unsigned char tmp1;

	size_t i = 0;
	if(sizeInBytes > sizeof(X_X_LONGEST)){
		do{

			tmp0 = a[i];
			a[i] = b[i];
			b[i] = tmp0;

			sizeInBytes-=sizeof(X_X_LONGEST);
			i+=1;
		}while(sizeInBytes > sizeof(X_X_LONGEST));
		i*=sizeof(X_X_LONGEST);
	}

    //*
	while(sizeInBytes){
        tmp1 = ((unsigned char *)a)[i];
			((unsigned char *)a)[i] = ((unsigned char *)b)[i];
			((unsigned char *)b)[i] = tmp1;
		i++;
        sizeInBytes--;
	}
    //*/
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

    //*
	while(sizeInBytes){
        tmp1 = ((unsigned char *)a)[i];
			((unsigned char *)a)[i] = ((unsigned char *)b)[i];
			((unsigned char *)b)[i] = tmp1;
		i++;
        sizeInBytes--;
	}
    //*/
}

void OBC_memcpyAVX(X_X_LONGEST* __restrict b, const X_X_LONGEST* __restrict a, size_t sizeInBytes)
{
	X_X_LONGEST tmp0;

	size_t i = 0;
	if(sizeInBytes > sizeof(X_X_LONGEST)){
		do{

			tmp0 = a[i];

			b[i] = tmp0;

			sizeInBytes-=sizeof(X_X_LONGEST);
			i+=1;
		}while(sizeInBytes > sizeof(X_X_LONGEST));
	}

	i*=sizeof(X_X_LONGEST);
	while(sizeInBytes){
		((unsigned char *)b)[i] = ((unsigned char *)b)[i];
		i++;
        sizeInBytes--;
	}
}

void memswap(X_X_LONGEST* __restrict b, X_X_LONGEST* __restrict a, size_t sizeInBytes)
{

	size_t i = 0;
	if(sizeInBytes >= sizeof(X_X_LONGEST)*4){
        sizeInBytes-=sizeof(X_X_LONGEST)*4;
		do{

			X_X_LONGEST tmp0 = a[i];
			X_X_LONGEST tmp1 = a[i+1];
			X_X_LONGEST tmp2 = a[i+2];
			X_X_LONGEST tmp3 = a[i+3];

            a[i] = b[i];
			a[i+1] = b[i+1];
			a[i+2] = b[i+2];
			a[i+3] = b[i+3];

			b[i] = tmp0;
			b[i+1] = tmp1;
			b[i+2] = tmp2;
			b[i+3] = tmp3;

			sizeInBytes-=sizeof(X_X_LONGEST)*4;
			i+=4;
		}while(sizeInBytes > sizeof(X_X_LONGEST)*4);
		i*=sizeof(X_X_LONGEST);
	}

	//*
	if(sizeInBytes){
        while(sizeInBytes){
            ((unsigned char *)b)[i] = ((unsigned char *)b)[i];
            i++;
            sizeInBytes--;
        }
	}
	//*/
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



	//*
	if(sizeInBytes){
        while(sizeInBytes){
            ((unsigned char *)b)[i] = ((unsigned char *)b)[i];
            i++;
            sizeInBytes--;
        }
	}
	//*/
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

	//*
	if(sizeInBytes){
        do{
            sizeInBytes--;
            unsigned char tmp0 = ((unsigned char *)a)[i];
            ((unsigned char *)a)[i] = ((unsigned char *)b)[i];
            ((unsigned char *)b)[i] = tmp0;
            i++;
        }while(sizeInBytes);
	}
	//*/
}


void OBC_swapX2(void * __restrict a, void * __restrict b, size_t sizeInBytes)
{
	X_X_LONGEST tmp0;
	unsigned char tmp1;

	size_t i = 0;
	size_t units = sizeInBytes/sizeof(X_X_LONGEST);

	if(units--){
        do{

			tmp0 = ((X_X_LONGEST *)a)[units];
			((X_X_LONGEST *)a)[units] = ((X_X_LONGEST *)b)[units];
			((X_X_LONGEST *)b)[units] = tmp0;

		}while(units--);
	}

	size_t bytes = sizeInBytes&((((size_t)1)<<sizeof(X_X_LONGEST))-1);

    if(bytes--){
        do{
            const size_t charPos = sizeInBytes-bytes;
			tmp1 = ((unsigned char *)a)[charPos];
			((unsigned char *)a)[charPos] = ((unsigned char *)b)[charPos];
			((unsigned char *)b)[charPos] = tmp1;

		}while(bytes--);
	}

}

///https://db.in.tum.de/~finis/x86-intrin-cheatsheet-v2.1.pdf
/*
///https://codereview.stackexchange.com/questions/5520/copying-80-bytes-as-fast-as-possible
void memcpyv5(__m128* __restrict b, const __m128* __restrict a, size_t sizeInBytes)
{
	__m128 tmp0,tmp1,tmp2,tmp3;

	size_t i = 0;
	if(sizeInBytes > sizeof(__m128)*4){
		do{

			__m128 tmp0 = a[i];
			__m128 tmp1 = a[i+1];
			__m128 tmp2 = a[i+2];
			__m128 tmp3 = a[i+3];

			b[i] = tmp0;
			b[i+1] = tmp1;
			b[i+2] = tmp2;
			b[i+3] = tmp3;

			sizeInBytes-=sizeof(__m128)*4;
			i+=sizeof(__m128)*4;
		}while(sizeInBytes > sizeof(__m128)*4);
	}
}
*/
