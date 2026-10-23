cmake_minimum_required(VERSION 3.16)
project(embedded_cicd_pipeline VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_COVERAGE "Build with gcov coverage instrumentation" OFF)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

add_library(cicd_warnings INTERFACE)
target_compile_options(cicd_warnings INTERFACE
    -Wall -Wextra -Wpedantic
    $<$<BOOL:${WARNINGS_AS_ERRORS}>:-Werror>)

add_library(cicd_core
    src/moving_average.cpp
    src/signal_validator.cpp)
target_include_directories(cicd_core PUBLIC include)
target_link_libraries(cicd_core PRIVATE cicd_warnings)

if(ENABLE_COVERAGE)
    target_compile_options(cicd_core PUBLIC --coverage -O0 -g)
    target_link_libraries(cicd_core PUBLIC --coverage)
endif()

add_executable(signal_demo src/main.cpp)
target_link_libraries(signal_demo PRIVATE cicd_core cicd_warnings)

enable_testing()
add_subdirectory(tests)
