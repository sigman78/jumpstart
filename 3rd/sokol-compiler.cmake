############# Shader Compiler ##################

if(WIN32)
  set(_hostPathPrefix win32)
elseif(OSX)
  set(_hostPathPrefix osx)
elseif(UNIX)
  set(_hostPathPrefix linux)
else()
  message(FATAL_ERROR "Unknown host platform.")
endif()

#find_program(sokolCompiler NAMES sokol-shdc PATHS "${CMAKE_CURRENT_SOURCE_DIR}/sokol-tools-bin/bin/${_hostPathPrefix}" NO_DEFAULT_PATH)
add_executable(sokolCompiler IMPORTED GLOBAL)
set_target_properties(sokolCompiler PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/sokol-tools-bin/bin/${_hostPathPrefix}/sokol-shdc${CMAKE_EXECUTABLE_SUFFIX}")

# move to platform selection/detection
set(SOKOL_BACKEND SOKOL_GLCORE33)
set(SOKOL_SHADER_LANG glsl330)

#
# add_shaders(<TARGET> [LANGS <glsl330|glsl100|glsl300es|hlsl4|hlsl5|metal_macos|metal_ios|metal_sim|wgpu>] SHADERS <SRCS>)
function(add_shaders _Target)

if(NOT TARGET "${_Target}")
  message(FATAL_ERROR "add_shaders: first argument should specify existing target to add shaders to.")
endif()

cmake_parse_arguments(_Args "" "" "LANGS;SHADERS" "${ARGN}")

if(NOT _Args_LANGS)
  message(FATAL_ERROR "add_shaders: specify correct shaders backend with LANGS option.")
endif()

if(NOT _Args_SHADERS)
  message(FATAL_ERROR "add_shaders: needs one or more shader source files.")
endif()

set(_ShaderIncDir "${CMAKE_CURRENT_BINARY_DIR}/${_Target}_shaders")
file(MAKE_DIRECTORY ${_ShaderIncDir})

string(REPLACE ";" ":" _LangList "${_Args_LANGS}")

set(_AllShaders)

foreach(_Shader ${_Args_SHADERS})
  get_filename_component(_AbsPath ${_Shader} ABSOLUTE)
  get_filename_component(_Name ${_Shader} NAME_WE)

  set(_ShaderDst "${_ShaderIncDir}/${_Name}.shader.h")

  add_custom_command(
    OUTPUT ${_ShaderDst}
    COMMAND sokolCompiler
    ARGS "--slang=${_LangList}" "--input=${_AbsPath}" "--output=${_ShaderDst}"
    DEPENDS "${_AbsPath}" sokolCompiler
    COMMENT "Running shader compiler on ${_Name}."
    VERBATIM
  )

  list(APPEND _AllShaders "${_ShaderDst}")
endforeach()

message(STATUS "Generated shaders: ${_AllShaders}")

set_source_files_properties(${_AllShaders} PROPERTIES GENERATED TRUE)

target_sources(${_Target} PRIVATE ${_AllShaders} ${_Args_SHADERS})
target_include_directories(${_Target} PRIVATE ${_ShaderIncDir})
source_group("Shaders" FILES ${_Args_SHADERS})
source_group("Shaders\\Generated" FILES ${_AllShaders})
  
endfunction()
