cmake_minimum_required(VERSION 3.17)

project(Jambiani LANGUAGES C CXX VERSION 0.0.1)

#add_subdirectory(3rd)
#add_subdirectory(src)

#include(3rd/libraries.cmake)

enable_testing()

add_subdirectory(3rd)
add_subdirectory(src)
add_subdirectory(test)

#include(src/projects.cmake)
