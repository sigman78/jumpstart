//
//

#pragma once

#cmakedefine PLATFORM_TARGET_ANDROID
#cmakedefine PLATFORM_TARGET_IOS
#cmakedefine PLATFORM_TARGET_APPLE
#cmakedefine PLATFORM_TARGET_MACOS
#cmakedefine PLATFORM_TARGET_WINDOWS
#cmakedefine PLATFORM_TARGET_LINUX
#cmakedefine PLATFORM_TARGET_LINUX_LIKE
#cmakedefine PLATFORM_TARGET_DESKTOP
#cmakedefine PLATFORM_TARGET_MOBILE
#cmakedefine PLATFORM_TARGET_EMSCRIPTEN
#cmakedefine PLATFORM_TARGET_WEB

#if defined(__i386) || defined(__x86_64) || defined(_M_IX86) || defined(_M_X64)
#define PLATFORM_TARGET_X86
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
#define PLATFORM_TARGET_ARM
#elif defined(__powerpc64__) || defined(_M_PPC)
#define PLATFORM_TARGET_POWERPC
#endif

#if defined(PLATFORM_TARGET_EMSCRIPTEN) && (defined(PLATFORM_TARGET_X86) || defined(PLATFORM_TARGET_ARM))
#error PLATFORM_TARGET_X86 / _ARM / _POWERPC defined on Emscripten
#endif

// C++ standard
#ifdef _MSC_VER
#ifdef _MSVC_LANG
#define PLATFORM_CXX_STANDARD _MSVC_LANG
#else
#define PLATFORM_CXX_STANDARD 201103L
#endif
#else
#define PLATFORM_CXX_STANDARD __cplusplus
#endif

// STL edition
//   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65473
//   https://github.com/gcc-mirror/gcc/commit/19665740d336d4ee7d0cf92b5b0643fa1d7da14a
//   https://en.cppreference.com/w/cpp/header/ciso646 */
#include <ciso646>
#ifdef _LIBCPP_VERSION
#define PLATFORM_TARGET_LIBCXX
#elif defined(_CPPLIB_VER)
#define PLATFORM_TARGET_DINKUMWARE
#elif defined(__GLIBCXX__)
#define PLATFORM_TARGET_LIBSTDCXX
#elif defined(__has_include)
    #if __has_include(<bits/c++config.h>)
        #include <bits/c++config.h>
        #ifdef __GLIBCXX__
        #define PLATFORM_TARGET_LIBSTDCXX
        #endif
    #endif
#elif defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5
#define PLATFORM_TARGET_LIBSTDCXX
#else
#endif

#ifdef __GNUC__
#define PLATFORM_TARGET_GCC
#endif

#ifdef __clang__
#define PLATFORM_TARGET_CLANG
#endif

#if defined(__clang__) && defined(_MSC_VER)
#define PLATFORM_TARGET_CLANG_CL
#endif

#if defined(__clang__) && defined(__apple_build_version__)
#define PLATFORM_TARGET_APPLE_CLANG
#endif

#ifdef _MSC_VER
#define PLATFORM_TARGET_MSVC
#endif

#ifdef __MINGW32__
#define PLATFORM_TARGET_MINGW
#endif

// Endianess
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define PLATFORM_TARGET_BIG_ENDIAN
#elif __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error what kind of endianness is this?
#endif
#elif defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MIPSEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define PLATFORM_TARGET_BIG_ENDIAN
#endif

#if defined(PLATFORM_TARGET_MSVC) || (defined(PLATFORM_TARGET_ANDROID) && !__LP64__) || defined(PLATFORM_TARGET_EMSCRIPTEN)
#define PLATFORM_LONG_DOUBLE_SAME_AS_DOUBLE
#endif
