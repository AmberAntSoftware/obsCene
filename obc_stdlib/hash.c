#include <math.h>
#include "hash.h"
#include "random.h"
//197               0xC5
//509               0x1D
//2039              0x7F7
//536870909         0x1FFFFFFD
//4294967291        0xFFFFFFFB
#define HASH_PRIME ((OBC_Hash)0x1FFFFFFD)
#define HASH_MAX_GENERATION_ATTEMPTS 500
#define HASH_MAX_RABINMILLER_ATTEMPTS 100

//alt
OBC_Hash OBC_hash(const void *const data, const size_t sizeInBytes){

    OBC_Hash hash = 0;
    size_t i;
    const unsigned char *const bytes = (const unsigned char *const)data;

    for(i = 0; i < sizeInBytes; i++){
        hash = HASH_PRIME * hash + bytes[i];
    }

    return hash;
}

//obc
OBC_Hash OBC_hash2(const void *const data, const size_t sizeInBytes){

    OBC_Hash hash = 1;
    size_t midHash;
    size_t i;
    const size_t chunks = (sizeInBytes/4)*4;
    const unsigned char *const bytes = (const unsigned char *const)data;

    if(sizeInBytes > sizeof(size_t)){
        for(i = 0; i < chunks; i+=4){
            midHash = 1;

            midHash|=bytes[i];
            midHash<<=8;
			midHash|=bytes[i+1];
			midHash<<=8;
			midHash|=bytes[i+2];
			midHash<<=8;

			midHash|=bytes[i+3];
			hash+=midHash*midHash*midHash+midHash;
        }
    }

    for(; i <sizeInBytes; i++) {
        hash+=bytes[i]*bytes[i];
    }

    return hash;
}

//https://www.baeldung.com/java-hashcode
//https://primes.utm.edu/lists/2small/0bit.html

/*
//https://en.wikipedia.org/wiki/Hash_function
//http://lcm.csa.iisc.ernet.in/dsa/node44.html
//https://stackoverflow.com/questions/11871245/knuth-multiplicative-hash
unsigned int hash3(unsigned int valueToHash, unsigned int relativePrimeToW, unsigned int ) {
    const unsigned int w = sizeof(unsigned int)*CHAR_BIT;
    valueToHash ^= valueToHash >> (w-m);
    return (relativePrime*valueToHash) >> (w-m)
}
//*/


OBC_Hash OBC_hashCombine(OBC_Hash hash1, OBC_Hash hash2){

    return (HASH_PRIME * hash1) + hash2;

}



OBC_Hash OBC_hashGenerateLikelyPrime(){

    OBC_Hash randHash;
    size_t tries;

    for(tries = 0; tries < HASH_MAX_GENERATION_ATTEMPTS; tries++){

        do{
            randHash = OBC_hashRandom();
        }while(randHash < 31);
        //randHash&=((~((size_t)0))-1);
        randHash|=1<<(sizeof(OBC_Hash)*CHAR_BIT-1);
        randHash|=1;
        printf("Trying:  %u...\n",randHash);


        if(OBC_hashGeneratePrimeMillerRabn(randHash,5)==1){
            return randHash;
        }



        randHash = 0;

    }

    return randHash;

}

char OBC_hashGeneratePrimeMillerRabn(OBC_Hash hash,int testCount){

    OBC_Hash randA;

    OBC_Hash sBits = 0;
    OBC_Hash rVal = hash - 1;
    while((rVal&1) == 0){
        sBits++;
        rVal/=2;
    }

    long long unsigned int base;
    size_t i,j;
    for(i = 0; i < testCount; i++){

        do{
            randA = OBC_hashRandom()%(hash-1);
        }while(randA < 2);

        /*
        double a = (double)randA;
        double r = (double)rVal;
        double m = (double)hash;
        base = (long long unsigned int)ceil( fmod( (pow(a,r)) ,m) );
        /*/
        base = ipow(randA, rVal);
        //*/
        base%=hash;
        printf("hash: %u\n",hash);
        printf("init rand: %u\n",randA);
        printf("init poq: %u\n",rVal);
        ///printf("pow: %f\n",pow(a,r));
        ///printf("fmod: %f\n",fmod( (pow(a,r)) ,m));
        //printf("init a: %f\n",a);
        //printf("init r: %f\n",r);
        printf("init base: %llu\n",base);

        if ((base != 1) && (base != hash - 1)){
            j=1;
            while ((j < sBits) && (base != hash - 1)){
                //base = (base*base) % hash;
                base = (base*base)%hash;
                if (base == 1){
                    return 0;
                }
                j++;
            }
            if (base != hash - 1){
                return 0;
            }
        }
    }

    return 1;
}


OBC_Hash OBC_hashRandom(){

    OBC_Hash hash = 0;
    size_t i;
    for(i = 0; i < sizeof(size_t); i+=sizeof(int)){
        hash<<=(CHAR_BIT*sizeof(int));
        //int val = rand();
        int val = RNG_rand();
        hash^=val;
    }

    return hash;
}

unsigned long long int ipow(unsigned long long int base, unsigned long long int exp)
{
    unsigned long long int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}
