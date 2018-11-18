cmake_minimum_required(VERSION 3.12)
project(Heap)

set(CMAKE_CXX_STANDARD 14)

add_executable(Heap main.cpp src/Heap.h tests/Test.cpp)

add_subdirectory(lib/googletest-master)

include_directories(lib/googletest-master/googletest/include)

include_directories(lib/googletest-master/googlemock/include)

target_link_libraries(Heap gtest gtest_main)