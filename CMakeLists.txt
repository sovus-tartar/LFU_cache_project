cmake_minimum_required (VERSION 3.13.4)
project(lfu)

set (CMAKE_BUILD_TYPE Debug)

set (SOURCES source/main.cpp source/hashmap.cpp source/lfu_func.cpp source/list.cpp)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE headers)

add_compile_options (-g -Werror -Wall -Wextra -Wpedantic)