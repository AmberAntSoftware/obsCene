#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <stdint.h>

void RNG_seed(int seed);
int RNG_rand();

void RNG_seedLLU(unsigned long long int seedLLU);
unsigned long long int RNG_rand64();

#endif // RANDOM_H_INCLUDED
