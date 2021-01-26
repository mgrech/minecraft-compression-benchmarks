set(BUILD_GMOCK OFF CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG release-1.10.0)

FetchContent_MakeAvailable(googletest)

add_executable(tests bitpacking.cpp palettization.cpp)
target_link_libraries(tests gtest gtest_main)
