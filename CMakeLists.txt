cmake_minimum_required(VERSION 3.8)
project(aligned_circular_buffer)

include_directories("../Catch/single_include/")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp aligned_circular_buffer.cpp aligned_circular_buffer.hpp tests/aligned_circular_buffer_test.cpp)
add_executable(aligned_circular_buffer ${SOURCE_FILES})