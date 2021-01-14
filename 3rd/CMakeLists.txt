#

include(CMakeParseArguments)

add_library(stbLib INTERFACE)
#target_include_directories(stbLib SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")
set_target_properties(stbLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/stb/")

#add_library(imguiLib INTERFACE)
#target_include_directories(imguiLib SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
#set_target_properties(imguiLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")

add_library(imguiLib STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)
target_include_directories(imguiLib SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

include(sokol-lib.cmake)
include(sokol-compiler.cmake)
add_subdirectory(glm)
