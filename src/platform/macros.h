//
//
#pragma once

#include "defines.h"

#define _IMPL_MACRO_CONCAT(a, b) a ## b
#define _MACRO_CONCAT(a, b) _IMPL_MACRO_CONCAT(a, b)
#define _MACRO_STR(x) #x

#define _LINE_STRING_IMPLEMENTATION(...) _MACRO_STR(__VA_ARGS__)

// Unused var
#if defined(PLATFORM_TARGET_GCC) || defined(PLATFORM_TARGET_CLANG_CL)
#define PLATFORM_UNUSED __attribute__((__unused__))
#elif defined(PLATFORM_TARGET_MSVC)
#define PLATFORM_UNUSED __pragma(warning(suppress:4100))
#else
#define PLATFORM_UNUSED
#endif

// Switch-case fall-through
#if defined(PLATFORM_TARGET_GCC) && __GNUC__ >= 7
#define PLATFORM_FALLTHROUGH __attribute__((fallthrough));
#elif defined(PLATFORM_TARGET_CLANG)
#define PLATFORM_FALLTHROUGH [[clang::fallthrough]];
#else
#define PLATFORM_FALLTHROUGH
#endif

// Alignment
#if defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ < 408
#define PLATFORM_ALIGNAS(alignment) __attribute__((aligned(alignment)))
#else
#define PLATFORM_ALIGNAS(alignment) alignas(alignment)
#endif

// No return
#if defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ < 408
#define PLATFORM_NORETURN __attribute__((noreturn))
#else
#define PLATFORM_NORETURN [[noreturn]]
#endif

// Thread local
#ifdef __has_feature
#if !__has_feature(cxx_thread_local)
#define PLATFORM_THREAD_LOCAL __thread
#endif
#endif
#ifndef PLATFORM_THREAD_LOCAL
#define PLATFORM_THREAD_LOCAL thread_local
#endif

// Inline always
#ifdef __GNUC__
#define PLATFORM_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define PLATFORM_ALWAYS_INLINE __forceinline
#else
#define PLATFORM_ALWAYS_INLINE inline
#endif

// Prevent inline
#ifdef __GNUC__
#define PLATFORM_NEVER_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define PLATFORM_NEVER_INLINE __declspec(noinline)
#else
#define PLATFORM_NEVER_INLINE
#endif

// Function name
#ifndef PLATFORM_TARGET_ANDROID
#define PLATFORM_FUNCTION __func__
#else
#define PLATFORM_FUNCTION __FUNCTION__
#endif

// __LINE__ as a string
#define PLATFORM_LINE_STRING _LINE_STRING_IMPLEMENTATION(__LINE__)

// Like possibly more convenient to debug version of std::abort()
#ifndef PLATFORM_ABORT
#ifndef __has_builtin
  #define __has_builtin(x) 0
#endif
#if defined(_MSC_VER)
#define PLATFORM_ABORT() __debugbreak()
#elif defined(__clang__) && __has_builtin(__builtin_debugtrap)
#define PLATFORM_ABORT() __builtin_debugtrap()
#elif defined(__GNUC__)
#define PLATFORM_ABORT() __builtin_trap()
#else
#define PLATFORM_ABORT() abort() // or (static_cast<void>(0))
#endif
#undef __has_builtin
#endif

// Call function before main
#ifndef PLATFORM_AUTOMATIC_INITIALIZER
#define PLATFORM_AUTOMATIC_INITIALIZER(function) \
    namespace { \
        struct Initializer_##function { static const int i; }; \
        const int Initializer_##function::i = function(); \
    } \
    /* */
#endif

// Call function after main
#ifndef PLATFORM_AUTOMATIC_FINALIZER
#define PLATFORM_AUTOMATIC_FINALIZER(function) \
    class Finalizer_##function { \
        public: \
            Finalizer_##function() {} \
            ~Finalizer_##function() { function(); } \
    } Finalizer_##function; \
    /* */
#endif
