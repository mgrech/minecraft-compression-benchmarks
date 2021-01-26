cmake_minimum_required(VERSION 3.16)
project(minecraft-compression-benchmarks)

option(BUILD_TESTS OFF)

set(CMAKE_CXX_STANDARD 17)

include_directories(external)
add_executable(bench main.cpp)
target_link_libraries(bench z deflate zstd lz4 brotlienc bz2)

if(BUILD_TESTS)
	add_subdirectory(tests)
endif()
