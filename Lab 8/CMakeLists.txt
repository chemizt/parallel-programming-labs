cmake_minimum_required(VERSION 3.15)

project(lab_8)

set(SOURCE_MAIN src/main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_CXX_FLAGS "-O3 -Os -Wall -Wextra -Wno-unused-value -Wno-deprecated-declarations -pedantic -fopenmp -std=c++2a")

include_directories(src/utils/include)

add_subdirectory(src/utils)

add_executable(main ${SOURCE_MAIN})

target_link_libraries(main utils) 
