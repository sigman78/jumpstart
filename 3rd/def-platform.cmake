#

set(BUILD_OS_ANDROID Android)
set(BUILD_OS_IOS iOS)
set(BUILD_OS_APPLE iOS Darwin)
set(BUILD_OS_MACOS Darwin)
set(BUILD_OS_WINDOWS Windows WindowsStore)
set(BUILD_OS_LINUX Linux)
set(BUILD_OS_LINUX_LIKE Android Linux)
set(BUILD_OS_DESKTOP Darwin Windows WindowsStore Linux)
set(BUILD_OS_MOBILE Android iOS)
set(BUILD_OS_EMSCRIPTEN Emscripten)
set(BUILD_OS_WEB Emscripten)

set(BUILD_OS ${CMAKE_SYSTEM_NAME})
set(HOST_OS ${CMAKE_HOST_SYSTEM_NAME})

if(HOST_OS IN_LIST BUILD_OS_WINDOWS)
    set(HOST_OS_SUFFIX "win32")
elseif(HOST_OS IN_LIST BUILD_OS_MACOS)
    set(HOST_OS_SUFFIX "osx")
elseif(HOST_OS IN_LIST BUILD_OS_LINUX)
    set(HOST_OS_SUFFIX "linux")
else()
    message(FATAL_ERROR "Unsupported host os: ${HOST_OS}")
endif()

# generate defines
macro(platform_os_define _Check _Def)
	if(BUILD_OS IN_LIST ${_Check})
		set(${_Def} 1)
	endif()
endmacro()

platform_os_define(BUILD_OS_ANDROID		PLATFORM_TARGET_ANDROID)
platform_os_define(BUILD_OS_IOS			PLATFORM_TARGET_IOS)
platform_os_define(BUILD_OS_APPLE		PLATFORM_TARGET_APPLE)
platform_os_define(BUILD_OS_MACOS		PLATFORM_TARGET_MACOS)
platform_os_define(BUILD_OS_WINDOWS		PLATFORM_TARGET_WINDOWS)
platform_os_define(BUILD_OS_LINUX		PLATFORM_TARGET_LINUX)
platform_os_define(BUILD_OS_LINUX_LIKE	PLATFORM_TARGET_LINUX_LIKE)
platform_os_define(BUILD_OS_DESKTOP		PLATFORM_TARGET_DESKTOP)
platform_os_define(BUILD_OS_MOBILE		PLATFORM_TARGET_MOBILE)
platform_os_define(BUILD_OS_EMSCRIPTEN	PLATFORM_TARGET_EMSCRIPTEN)
platform_os_define(BUILD_OS_WEB			PLATFORM_TARGET_WEB)

set(PLATFORM_CONFIG_DIR "${CMAKE_BINARY_DIR}/gen")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake ${PLATFORM_CONFIG_DIR}/configure.h)



