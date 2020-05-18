#ifndef OBC_NATIVES_H_INCLUDED
#define OBC_NATIVES_H_INCLUDED

//https://www.linuxquestions.org/questions/ubuntu-63/using-xmmintrin-h-in-ubuntu-707942/
///AVX bitcount, unknown defaults to 128 max -- values of: 128 or 256 or 512
///add the compiler flags -march=native  OR -mavx2 -mavx -mfma
///512 is more rare (2019) and can be significantly slower than 128bit
#define AVX_BIT_SIZE 256
#define SIMD_BIT_SIZE AVX_BIT_SIZE



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

#ifndef OBC_LONGEST
#ifdef AVX_BIT_SIZE
#ifdef __AVX__

    #if AVX_BIT_SIZE == 512
    #elif AVX_BIT_SIZE == 256
    #elif AVX_BIT_SIZE == 128
    #else
        #undef AVX_BIT_SIZE
        #define AVX_BIT_SIZE 128
    #endif // AVX_BIT_SIZE value


    #ifndef OBC_LONGEST
        #ifdef __FMA__
            #if AVX_BIT_SIZE == 512
                //https://software.intel.com/en-us/forums/intel-isa-extensions/topic/705896
                #define OBC_LONGEST __m512i
            #endif // AVX_BIT_SIZE
        #endif // __AVX512CD__
    #endif // OBC_LONGEST

    #ifndef OBC_LONGEST
        #ifdef __AVX2__
            #if AVX_BIT_SIZE == 256 || AVX_BIT_SIZE == 512
                #define OBC_LONGEST __m256i
            #endif // AVX_BIT_SIZE
        #endif // __AVX2__
    #endif // OBC_LONGEST

    #ifndef OBC_LONGEST
        #ifdef __AVX__
            #if AVX_BIT_SIZE == 128 || AVX_BIT_SIZE == 256 || AVX_BIT_SIZE == 512
                #define OBC_LONGEST __m128i
            #endif // AVX_BIT_SIZE
        #endif // __AVX__
    #endif // OBC_LONGEST

#endif // __AVX__
#endif // AVX_BIT_SIZE
#endif // X_X_LONGEST

#ifndef OBC_LONGEST
    #define OBC_LONGEST long long unsigned int
#endif // X_X_LONGEST

#endif // OBC_NATIVES_H_INCLUDED
