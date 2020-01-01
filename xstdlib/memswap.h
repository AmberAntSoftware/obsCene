#ifndef MEMSWAP_H_INCLUDED
#define MEMSWAP_H_INCLUDED

#include <string.h>

//https://sourceforge.net/p/predef/wiki/Architectures/
#ifndef AVX_HEADER_CHECK
    #ifdef __i386__
        #include <immintrin.h>
    #endif
    #ifdef __i386
        #include <immintrin.h>
    #endif
    #ifdef _M_IX86
        #include <immintrin.h>
    #endif
    #ifdef __INTEL__
        #include <immintrin.h>
    #endif
    #ifdef __I86__
        #include <immintrin.h>
    #endif
    #ifdef _X86_
        #include <immintrin.h>
    #endif
    #ifdef __X86__
        #include <immintrin.h>
    #endif
    #ifdef _X86_
        #include <immintrin.h>
    #endif
    #ifdef __x86_64__
        #include <immintrin.h>
    #endif
    #ifdef __x86_64
        #include <immintrin.h>
    #endif
    #ifdef _M_AMD64
        #include <immintrin.h>
    #endif
    #ifdef __amd64__
        #include <immintrin.h>
    #endif
    #ifdef __amd64
        #include <immintrin.h>
    #endif
#define AVX_HEADER_CHECK
#endif // AVX_HEADER_CHECK
//128 is usually allowed from xmmintrin.h
//breaks on non x86 architectures

//https://www.linuxquestions.org/questions/ubuntu-63/using-xmmintrin-h-in-ubuntu-707942/
///AVX bitcount, unknown defaults to 128 max -- values of: 128 or 256 or 512
///add the compiler flags -march=native  OR -mavx2 -mavx -mfma
///512 is more rare (2019) and can be significantly slower than 128bit
#define AVX_BIT_SIZE 128
#ifdef AVX_BIT_SIZE
#ifdef __AVX__

    #if AVX_BIT_SIZE == 512
    #elif AVX_BIT_SIZE == 256
    #elif AVX_BIT_SIZE == 128
    #else
        #undef AVX_BIT_SIZE
        #define AVX_BIT_SIZE 128
    #endif // AVX_BIT_SIZE value


    #ifndef X_X_LONGEST
        #ifdef __FMA__
            #if AVX_BIT_SIZE == 512
                //https://software.intel.com/en-us/forums/intel-isa-extensions/topic/705896
                #define X_X_LONGEST __m512
            #endif // AVX_BIT_SIZE
        #endif // __AVX512CD__
    #endif // OBC_X_CPY

    #ifndef X_X_LONGEST
        #ifdef __AVX2__
            #if AVX_BIT_SIZE == 256 || AVX_BIT_SIZE == 512
                #define X_X_LONGEST __m256
            #endif // AVX_BIT_SIZE
        #endif // __AVX2__
    #endif // OBC_X_CPY

    #ifndef X_X_LONGEST
        #ifdef __AVX__
            #if AVX_BIT_SIZE == 128 || AVX_BIT_SIZE == 256 || AVX_BIT_SIZE == 512
                #define X_X_LONGEST __m128
            #endif // AVX_BIT_SIZE
        #endif // __AVX__
    #endif // OBC_X_CPY

#endif // __AVX__
#endif // AVX_BIT_SIZE

#ifndef X_X_LONGEST
    #define X_X_LONGEST long long unsigned int
#endif // X_X_LONGEST

void *OBC_memswap (void * __restrict ptr0, void * __restrict ptr1, size_t sizeInBytes);
void OBC_swapX(void * __restrict a, void * __restrict b, size_t sizeInBytes);
void memswap2(X_X_LONGEST* __restrict b, X_X_LONGEST* __restrict a, size_t sizeInBytes);
void memswap3(X_X_LONGEST* __restrict b, X_X_LONGEST* __restrict a, size_t sizeInBytes);

#endif // MEMSWAP_H_INCLUDED
