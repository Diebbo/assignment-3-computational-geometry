cmake_minimum_required(VERSION 3.16)
project(ConvexHullProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# ---- Library with your algorithm ----
file(GLOB LIB_SOURCES src/*.cpp)
add_library(convex_hull_lib ${LIB_SOURCES})

if (MSVC)
    target_compile_options(convex_hull_lib PRIVATE /W4)
else()
    target_compile_options(convex_hull_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- Main executable ----
add_executable(convex_hull src/main.cpp)
target_link_libraries(convex_hull PRIVATE convex_hull_lib)

# ---- Google Benchmark ----
find_package(benchmark REQUIRED)

# ---- Benchmark target ----
file(GLOB BENCHMARK_SOURCES benchmarks/*.cpp)
add_executable(convex_hull_bench ${BENCHMARK_SOURCES})
target_link_libraries(convex_hull_bench PRIVATE convex_hull_lib benchmark::benchmark)
