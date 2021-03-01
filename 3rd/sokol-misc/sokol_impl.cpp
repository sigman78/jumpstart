// Sokol lib instatiation
// Note: Mac implementation needs this file compiled as OBJCPP

// Requires to define:
// - One of the sokol backends
// - SOKOL_BARE - to keep bottom leve API to minumum
// - SOKOL_TRACE_HOOKS - to use trace hooks and debug api

#define SOKOL_IMPL

// Also we should define one of the graphics backends
#if !defined(SOKOL_DUMMY_BACKEND) && !defined(SOKOL_GLCORE33) && !defined(SOKOL_GLES2) && !defined(SOKOL_GLES3) && !defined(SOKOL_D3D11) && !defined(SOKOL_METAL) && !defined(SOKOL_WGPU)
#error Non of the backends are configured!
#endif

#if !defined(SOKOL_BARE)

#include <platform/macros.h>
#include <platform/assert.h>
#define SOKOL_ASSERT(condition) PLATFORM_ASSERT((condition), _MACRO_STR(condition))
#define SOKOL_UNREACHABLE PLATFORM_ASSERT_UNREACHABLE()
#define SOKOL_LOG(msg) Platform::Debug{} << msg

#else

#if defined(_WIN32)
#define SOKOL_LOG(s) OutputDebugStringA(s)
#endif

#endif

// sokol 3D-API defines are provided by build options
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_time.h"
#include "sokol_audio.h"

#if defined(PLATFORM_TARGET_WEB)
#include "sokol_fetch.h"
#endif

#include "sokol_glue.h"
