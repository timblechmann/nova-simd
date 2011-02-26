cmake_minimum_required (VERSION 2.6)
project (nova-simd)

include(CTest)
enable_testing()

option(NATIVE "compile on gcc with -march=native" OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
    if (NATIVE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth-1000")
endif()

add_subdirectory (benchmarks)
add_subdirectory (testsuite)

