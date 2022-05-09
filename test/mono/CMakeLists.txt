add_executable(mono_test)
scan_sources(mono_test *.cpp *.h)

target_link_libraries(mono_test PRIVATE Mono doctest)

add_test(mono_test mono_test)