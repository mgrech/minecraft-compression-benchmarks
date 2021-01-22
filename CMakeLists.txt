cmake_minimum_required(VERSION 3.16)
project(minecraft-compression-benchmarks)

set(CMAKE_CXX_STANDARD 17)

include_directories(external)
add_executable(bench main.cpp)
target_link_libraries(bench deflate)
