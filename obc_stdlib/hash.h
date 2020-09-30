#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "../obc.h"

typedef OBC_Offset OBC_Hash;

OBC_Hash OBC_hash(const void *const data, const size_t sizeInBytes);
OBC_Hash OBC_hashX(const void *const data, const size_t sizeInBytes);

OBC_Hash OBC_hash2(const void *const data, const size_t sizeInBytes);

OBC_Hash OBC_hashReverse(OBC_Hash hash);
OBC_Hash OBC_hashCombine(OBC_Hash hash1, OBC_Hash hash2);

OBC_Hash OBC_hash3(const void *const data, const size_t sizeInBytes, unsigned char hashLimPoT);

/*
///prime generation
OBC_Hash OBC_hashRandom();
OBC_Hash OBC_hashGenerateLikelyPrime();
char OBC_hashPrimeMillerRabn(OBC_Hash hash,int testCount);
char OBC_hashGeneratePrimeMillerRabn(OBC_Hash hash,int testCount);

unsigned long long int ipow(unsigned long long int base, unsigned long long int exp);
//*/

#endif // HASH_H_INCLUDED
