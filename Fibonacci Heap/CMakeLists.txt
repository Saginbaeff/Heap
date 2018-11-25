cmake_minimum_required(VERSION 3.12)
project(Fibonacci_Heap)

set(CMAKE_CXX_STANDARD 14)

add_executable(Fibonacci_Heap main.cpp src/FibonacciHeap.h tests/Test.cpp)

add_subdirectory(lib/googletest-master)

include_directories(lib/googletest-master/googletest/include)

include_directories(lib/googletest-master/googlemock/include)

target_link_libraries(Fibonacci_Heap gtest gtest_main)