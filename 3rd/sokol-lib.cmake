include(def-platform.cmake)

set(SOKOL_PREFER_METAL OFF)

if(BUILD_OS IN_LIST BUILD_OS_WINDOWS)
    set(SOKOL_BACKENDS GLCORE33 GLES2 GLES3 D3D11 DUMMY)
elseif(BUILD_OS IN_LIST BUILD_OS_APPLE)
    set(SOKOL_BACKENDS GLCORE33 GLES2 GLES3 METAL DUMMY)
elseif(BUILD_OS IN_LIST BUILD_OS_LINUX)
    set(SOKOL_BACKENDS GLCORE33 GLES2 GLES3 DUMMY)
elseif(BUILD_OS IN_LIST BUILD_OS_WEB)
    set(SOKOL_BACKENDS GLES2 GLES3 DUMMY)
else()
    message(FATAL_ERROR "Unsupported OS ${BUILD_OS}")
endif()

set(SOKOL_SL_FOR_GLCORE33 glsl330)
set(SOKOL_SL_FOR_GLES2 glsl100)
set(SOKOL_SL_FOR_GLES3 glsl300es)
set(SOKOL_SL_FOR_D3D11 hlsl4 hlsl5)
set(SOKOL_SL_FOR_METAL metal_macos metal_ios metal_sim)

#Target shader languages (used with -l --slang):
#  - glsl330:       desktop OpenGL backend (SOKOL_GLCORE33)
#  - glsl100:       OpenGLES2 and WebGL (SOKOL_GLES2)
#  - glsl300es:     OpenGLES3 and WebGL2 (SOKOL_GLES3)
#  - hlsl4:         Direct3D11 with HLSL4 (SOKOL_D3D11)
#  - hlsl5:         Direct3D11 with HLSL5 (SOKOL_D3D11)
#  - metal_macos:   Metal on macOS (SOKOL_METAL)
#  - metal_ios:     Metal on iOS devices (SOKOL_METAL)
#  - metal_sim:     Metal on iOS simulator (SOKOL_METAL)
#  - wgpu:          output WebGPU compatible SPIR-V bytecode (SOKOL_WGPU)


add_library(sokolLib STATIC)

target_compile_definitions(sokolLib PUBLIC SOKOL_GLCORE33 USE_DBG_UI)
target_include_directories(sokolLib SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/sokol" "${CMAKE_CURRENT_SOURCE_DIR}/sokol/util")

if(BUILD_OS IN_LIST BUILD_OS_WINDOWS)
target_sources(sokolLib PRIVATE sokol-misc/sokol-lib.cpp)
elseif(BUILD_OS IN_LIST BUILD_OS_APPLE)
target_sources(sokolLib PRIVATE sokol-misc/sokol-lib.mm)
#set(SOKOL_IOS_FRAMEWORKS UIKit OpenGLES GLKit AudioToolbox)
set(SOKOL_OSX_FRAMEWORKS Cocoa QuartzCore OpenGL Metal MetalKit AudioToolbox)
foreach(_framework ${SOKOL_OSX_FRAMEWORKS})
    find_library(found_lib_${_framework} ${_framework})
    list(APPEND SOKOL_OSX_LIBS ${found_lib_${_framework}})
endforeach()
target_link_libraries(sokolLib PRIVATE ${SOKOL_OSX_LIBS})
else()
message(FATAL_ERROR "Unsupported platform: ${BUILD_OS}")
endif()

# TODO: Move outside
target_include_directories(sokolLib PUBLIC "${PLATFORM_CONFIG_DIR}")

#set_target_properties(sokolLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/sokol/;${CMAKE_CURRENT_SOURCE_DIR}/sokol/util/")
#add_library(sokol::lib ALIAS _sokolLib)

