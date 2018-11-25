cmake_minimum_required(VERSION 3.12)
project(BinomialHeap)

set(CMAKE_CXX_STANDARD 14)

add_executable(BinomialHeap main.cpp src/BinomialHeap.h tests/Test.cpp)

add_subdirectory(lib/googletest-master)

include_directories(lib/googletest-master/googletest/include)

include_directories(lib/googletest-master/googlemock/include)

target_link_libraries(BinomialHeap gtest gtest_main)