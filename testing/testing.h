#ifndef TESTING_H_INCLUDED
#define TESTING_H_INCLUDED

typedef enum OBC_TESTING_ENUM{
    OBC_TESTING_NO_COMPLETE,
    OBC_TESTING_SUCCESS,
    OBC_TESTING_FAILURE
}OBC_TESTING_ENUM;

///https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
///https://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch
/*
#ifdef __ANDROID__

#endif // __ANDROID__
#ifdef __linux__ && !__ANDROID__

#endif // __linux__
#ifdef _WIN64

#endif // _WIN64
#ifdef _WIN32

#endif // _WIN32
#ifdef __linux__

#endif // __linux__
#ifdef __sun

#endif // __sun
#ifdef __FreeBSD__

#endif // __FreeBSD__
#ifdef __NetBSD__

#endif // __NetBSD__
#ifdef __OpenBSD__

#endif // __OpenBSD__
#ifdef __APPLE__

#endif // __APPLE__
#ifdef __hpux

#endif // __hpux
#ifdef __osf__

#endif // __osf__
#ifdef __sgi

#endif // __sgi
#ifdef _AIX

#endif // _AIX
#ifdef __ros__

#endif // __ros__
#ifdef __native_client__

#endif // __native_client__
#ifdef __asmjs__

#endif // __asmjs__
#ifdef __Fuchsia__

#endif // __Fuchsia__
*/

#endif // TESTING_H_INCLUDED
