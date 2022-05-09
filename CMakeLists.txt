cmake_minimum_required(VERSION 3.17)

project(Jambiani LANGUAGES C CXX VERSION 0.0.1)
#add_subdirectory(3rd)
#add_subdirectory(src)

#include(3rd/libraries.cmake)

enable_testing()

function(scan_sources Target)
	if(NOT TARGET ${Target})
		message(FATAL_ERROR "Not a target")
	endif()
	set(Sources)
	foreach(Item ${ARGN})
		string(FIND ${Item} "*" AsteriskPos)
		if (AsteriskPos GREATER -1)
			# message(WARNING "Globbing ${Item}")
			file(GLOB_RECURSE FileList LIST_DIRECTORIES false RELATIVE "${CMAKE_CURRENT_LIST_DIR}" CONFIGURE_DEPENDS "${Item}")
			if(FileList)
				list(APPEND Sources "${FileList}")
			endif()
		else()
			list(APPEND Sources ${Item})
		endif()
		message(STATUS "${Sources}")
	endforeach()

	list(LENGTH Sources SourcesNum)
	if(SourcesNum LESS 1)
		message(WARNING "Empty sources list for ${Target}")
	endif()

	target_sources(${Target} PRIVATE "${Sources}")
endfunction()

add_subdirectory(3rd)
add_subdirectory(src)
add_subdirectory(test)

#include(src/projects.cmake)
