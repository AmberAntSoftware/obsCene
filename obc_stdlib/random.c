#include "random.h"


static unsigned int RNG_X_state = 0;
static unsigned long long int RNG_X_stateLLU = 0;

///https://rosettacode.org/wiki/Linear_congruential_generator#C
///https://en.wikipedia.org/wiki/Linear_congruential_generator
void RNG_seed(int seed){
    RNG_X_state = seed;
}
int RNG_rand(){

    const signed int RANDOM_MASK = ((signed int)~0);

    RNG_X_state = (RNG_X_state * 1103515245 + 12345) & RANDOM_MASK;

    //*
    signed int srand = 0;
    srand ^= RNG_X_state;
    return srand;
    /*/
    return RNG_X_state;
    //*/
}


void RNG_seedLLU(unsigned long long int seedLLU){
    RNG_X_stateLLU = seedLLU;
}

unsigned long long int RNG_rand64(){

    const unsigned long long int RANDOM_MASK = ~((unsigned long long int)0);

    RNG_X_stateLLU = (RNG_X_stateLLU * 6364136223846793005 + 1442695040888963407) & RANDOM_MASK;

    unsigned long long int srand = 0;
    srand ^= RNG_X_stateLLU;

    return srand;
}



/**

///https://en.wikipedia.org/wiki/Permuted_congruential_generator

#define unsigned long long int llu
#define unsigned int ui
static llu       state      = 0;		// Or something seed-dependent
static llu const multiplier = 6364136223846793005u;
static llu const increment  = 1442695040888963407u;	// Or an arbitrary odd constant

static int rotr32(uint32_t x, unsigned r)
{
	return x >> r | x << (-r & 31);
}

uint32_t pcg32(void)
{
	uint64_t x = state;
	ui count = (ui)(x >> 59);		// 59 = 64 - 5

	state = x * multiplier + increment;
	x ^= x >> 18;								// 18 = (64 - 27)/2
	return rotr32((ui)(x >> 27), count);	// 27 = 32 - 5
}

void pcg32_init(uint64_t seed)
{
	state = seed + increment;
	(void)pcg32();
}
//**/
