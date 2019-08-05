cmake_minimum_required(VERSION 2.8)
project(test)
set(CMAKE_CXX_STANDARD 11)


add_executable(fraction_test fraction_test.cpp)
target_link_libraries(fraction_test gtest_main)