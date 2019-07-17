cmake_minimum_required(VERSION 2.8)
project(Leetcode)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(p2_add_two_number)
add_subdirectory(p8_string_to_integer)

include_directories(include)

add_subdirectory(p3_longest_lubstring_without_repeating_characters)
add_executable(Leetcode main.cpp)