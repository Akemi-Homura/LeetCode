cmake_minimum_required(VERSION 2.8)
project(Leetcode)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(p2_add_two_number)
add_subdirectory(p8_string_to_integer)
add_subdirectory(p65_valid_number)
add_subdirectory(p151_reverse_words_in_string)
add_subdirectory(p149_max_points_at_line)
add_subdirectory(p3_longest_lubstring_without_repeating_characters)


add_executable(Leetcode main.cpp)