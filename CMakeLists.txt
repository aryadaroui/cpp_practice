cmake_minimum_required(VERSION 3.0.0)
project(cpp_practice VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ ISO Standard")

set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cpp_practice src/main.cpp)
target_include_directories(cpp_practice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message("C++ compiler: ${CMAKE_CXX_COMPILER}")
message("C++ version: ${CMAKE_CXX_STANDARD}")
message("C++ flags: ${CMAKE_CXX_FLAGS}")
