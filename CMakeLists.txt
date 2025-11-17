cmake_minimum_required(VERSION 3.16)
project(ConvexHullProject LANGUAGES CXX)

# ---- Compilation flags ----

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
endif()

if (SANITIZE_UNDEFINED)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

# ---- Dependencies ----
find_package(benchmark REQUIRED)

# ---- Library ----
file(GLOB LIB_SOURCES src/*.cpp)
add_library(hullib ${LIB_SOURCES})

# ---- Main executable ----
add_executable(convex_hull src/bin/main.cpp)
target_link_libraries(convex_hull PRIVATE hullib)

# ---- Benchmarks ----
add_executable(bench benchmarks/bench.cpp)
target_link_libraries(bench PRIVATE hullib benchmark::benchmark)
