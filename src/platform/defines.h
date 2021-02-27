//
//

#pragma once

#undef PLATFORM_TARGET_ANDROID
#undef PLATFORM_TARGET_IOS
#undef PLATFORM_TARGET_APPLE
#undef PLATFORM_TARGET_MACOS
#undef PLATFORM_TARGET_WINDOWS
#undef PLATFORM_TARGET_LINUX
#undef PLATFORM_TARGET_LINUX_LIKE
#undef PLATFORM_TARGET_DESKTOP
#undef PLATFORM_TARGET_MOBILE
#undef PLATFORM_TARGET_EMSCRIPTEN
#undef PLATFORM_TARGET_WEB

// Windows
#if defined(_MSC_VER) // __clang__ or __MINGW__ also has this i think
#define PLATFORM_TARGET_WINDOWS 1
#define PLATFORM_TARGET_DESKTOP 1
#endif

// Apple
#if !(defined(ANDROID) || defined(__ANDROID__)) && ((defined __APPLE__) || (defined __APPLE_CPP__) || (defined __APPLE_CC__) || (defined __MACOS_CLASSIC__))
// bring platform defines. it does pollute namespace though
#include <TargetConditionals.h>
#define PLATFORM_TARGET_APPLE 1

#if (TARGET_OS_MAC && !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE)
#define PLATFORM_TARGET_MACOS 1
#define PLATFORM_TARGET_DESKTOP 1
#endif

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#define PLATFORM_TARGET_IOS 1
#define PLATFORM_TARGET_MOBILE 1
#endif

#endif

// Android
#if defined(ANDROID) || defined(__ANDROID__)
#define PLATFORM_TARGET_ANDROID 1
#define PLATFORM_TARGET_MOBILE 1
#endif

// Linux
#if !(defined(ANDROID) || defined(__ANDROID__) || defined(__EMSCRIPTEN__)) && (defined(__linux__ ) || defined(__unix__))
#define PLATFORM_TARGET_LINUX 1
#define PLATFORM_TARGET_DESKTOP 1
#endif

// Emscripten
#if defined(__EMSCRIPTEN__)
#define PLATFORM_TARGET_EMSCRIPTEN 1
#define PLATFORM_TARGET_WEB 1
#endif

// Architecture
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

// Normalize and validate DEBUG/NDEBUG
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG 1
#endif
#if !defined(DEBUG) && !defined(NDEBUG)
#define NDEBUG 1 // Or DEBUG by default?
#endif
#if defined(DEBUG) && defined(NDEBUG)
#error Both NDEBUG and DEBUG errorneously defined!
#endif

// C++ standard
#if defined(_MSC_VER)
#if defined(_MSVC_LANG)
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
#if defined(_LIBCPP_VERSION)
#define PLATFORM_TARGET_LIBCXX
#elif defined(_CPPLIB_VER)
#define PLATFORM_TARGET_DINKUMWARE
#elif defined(__GLIBCXX__)
#define PLATFORM_TARGET_LIBSTDCXX
#elif defined(__has_include)
    #if __has_include(<bits/c++config.h>)
        #include <bits/c++config.h>
        #if defined(__GLIBCXX__)
        #define PLATFORM_TARGET_LIBSTDCXX
        #endif
    #endif
#elif defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5
#define PLATFORM_TARGET_LIBSTDCXX
#else
#endif

// Compiler type
#if defined(__GNUC__)
#define PLATFORM_TARGET_GCC
#endif

#if defined(__clang__)
#define PLATFORM_TARGET_CLANG
#endif

#if defined(__clang__) && defined(_MSC_VER)
#define PLATFORM_TARGET_CLANG_CL
#endif

#if defined(__clang__) && defined(__apple_build_version__)
#define PLATFORM_TARGET_APPLE_CLANG
#endif

#if defined(_MSC_VER)
#define PLATFORM_TARGET_MSVC
#endif

#if defined(__MINGW32__)
#define PLATFORM_TARGET_MINGW
#endif

// Endianess
#if defined(__BYTE_ORDER__)
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
