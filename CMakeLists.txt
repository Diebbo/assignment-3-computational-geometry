cmake_minimum_required(VERSION 3.16)
project(ConvexHullProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ---- Library ----
file(GLOB LIB_SOURCES src/*.cpp)
add_library(hullib ${LIB_SOURCES})

# ---- Main executable ----
add_executable(convex_hull src/main.cpp)
target_link_libraries(convex_hull PRIVATE hullib)

# ---- Benchmarks ----
find_package(benchmark REQUIRED)

add_executable(quickhull_bench benchmarks/quickhull_bench.cpp)
target_link_libraries(quickhull_bench PRIVATE hullib benchmark::benchmark)
add_executable(graham_scan_bench benchmarks/graham_scan_bench.cpp)
target_link_libraries(graham_scan_bench PRIVATE hullib benchmark::benchmark)
add_executable(mbc_bench benchmarks/mbc_bench.cpp)
target_link_libraries(mbc_bench PRIVATE hullib benchmark::benchmark)
