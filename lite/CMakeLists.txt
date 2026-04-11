cmake_minimum_required(VERSION 3.23)
project(spdlog_lite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# spdlog_lite is header-only — just an interface library
add_library(spdlog_lite INTERFACE)
target_include_directories(spdlog_lite INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(spdlog_lite INTERFACE cxx_std_20)

# Example
option(SPDLOG_LITE_BUILD_EXAMPLE "Build spdlog_lite example" ON)
if(SPDLOG_LITE_BUILD_EXAMPLE)
    add_executable(example example/example.cpp)
    target_link_libraries(example PRIVATE spdlog_lite)
endif()

# Benchmarks
option(SPDLOG_LITE_BUILD_BENCH "Build spdlog_lite benchmarks" OFF)
if(SPDLOG_LITE_BUILD_BENCH)
    find_package(benchmark CONFIG)
    if(NOT benchmark_FOUND)
        message(STATUS "Downloading Google Benchmark")
        include(FetchContent)
        set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
        FetchContent_Declare(googlebenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v1.8.4)
        FetchContent_MakeAvailable(googlebenchmark)
    endif()

    add_executable(latency bench/latency.cpp)
    target_link_libraries(latency PRIVATE spdlog_lite benchmark::benchmark)
endif()
