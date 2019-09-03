cmake_minimum_required(VERSION 3.13.1)
project(main)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(main main.cpp)

target_link_libraries(main pthread)