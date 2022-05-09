cmake_minimum_required(VERSION 3.17)

project(Mono)

add_library(Mono)

scan_sources(Mono *.cpp engine/*)

target_include_directories(Mono PUBLIC ${SUBPROJECT_INCLUDE_DIR})

target_compile_features(Mono PUBLIC cxx_std_20)