#include "hash.h"

//197               0xC5
//509               0x1D
//2039              0x7F7
//536870909         0x1FFFFFFD
//4294967291        0xFFFFFFFB
#define HASH_PRIME ((OBC_Hash)0x1FFFFFFD)

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
