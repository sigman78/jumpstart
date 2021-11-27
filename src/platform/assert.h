//
//

#pragma once

#include "platform/defines.h"

#include "platform/debug.h"
#include "platform/macros.h"

// overridable
// PLATFORM_ASSERT              - define own macro with syntax ASSERT(expression, message, [return value])
// following defines control behavior
// PLATFORM_NO_ASSERT           - NOOP
// PLATFORM_STANDARD_ASSERT     - default to C assert function

#ifndef PLATFORM_ASSERT
#if defined(PLATFORM_NO_ASSERT) || (defined(PLATFORM_STANDARD_ASSERT) && defined(NDEBUG))
#define PLATFORM_ASSERT(expr, msg) do {} while(0)
#elif defined(PLATFORM_STANDARD_ASSERT)
#define PLATFORM_ASSERT(expr, msg) assert(expr)
#else
#define PLATFORM_ASSERT(expr, msg) \
do { \
    if(!(expr)) { \
        {!Platform::Error{Platform::Error::systemOutput()} << "ASSERT("#expr"):" << msg;} \
        PLATFORM_ABORT(); \
    } \
} while(0) \
/* */
#endif
#endif


// PLATFORM_ASSERT_UNREACHABLE  - mark unreachable code
#ifndef PLATFORM_ASSERT_UNREACHABLE
#if defined(PLATFORM_NO_ASSERT) || (defined(PLATFORM_STANDARD_ASSERT) && defined(NDEBUG))
#if defined(__clang__)
#define PLATFORM_ASSERT_UNREACHABLE() __builtin_assume(0)
#elif defined(__GNUC__)
#define PLATFORM_ASSERT_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define PLATFORM_ASSERT_UNREACHABLE() __assume(0)
#else
#define PLATFORM_ASSERT_UNREACHABLE() std::abort()
#endif
#elif defined(PLATFORM_STANDARD_ASSERT)
#define PLATFORM_ASSERT_UNREACHABLE() assert(!"Unreachable code")
#else
#define PLATFORM_ASSERT_UNREACHABLE() \
    do { \
        {!Platform::Error{Platform::Error::systemOutput()} << "Unreachable code";} \
        PLATFORM_ABORT(); \
    } while(0) \
    /* */
#endif
#endif


namespace Platform {

} // Platform
