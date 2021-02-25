add_executable(platform-test-debug test-debug.cpp)
#target_compile_definitions(platform-test-debug PRIVATE SOKOL_GLCORE33 USE_DBG_UI)
target_link_libraries(platform-test-debug PRIVATE doctest platformLib sokolLib)

