cmake_minimum_required(VERSION 3.10)
project(MatrixApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SOURCES ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/matrix.cpp)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build)

add_executable(matrix ${SOURCES})
target_compile_options(matrix PRIVATE -g -pthread) 
target_include_directories(matrix PRIVATE ${SOURCE_DIR})