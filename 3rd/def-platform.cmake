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

set(PLATFORM_CONFIG_DIR "${CMAKE_BINARY_DIR}/gen")


