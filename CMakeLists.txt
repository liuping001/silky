cmake_minimum_required(VERSION 2.8)
project(silky)
set(CMAKE_CXX_STANDARD 11)
include_directories(.)

add_executable(test_array_wrapper test/array_wrapper_test.cpp)
add_executable(test_define test/define_test.cpp)
add_executable(test_object_pool test/object_pool_test.cpp)
add_executable(test_scope_guard test/scope_guard_test.cpp)
add_executable(test_auto_register test/auto_register_test.cpp)
add_executable(test_string test/strong_string_test.cpp)
add_executable(test_timer test/timer_test.cpp)
add_executable(test_argparse test/argparse_test.cpp)