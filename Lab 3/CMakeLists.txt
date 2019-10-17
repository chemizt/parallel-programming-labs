cmake_minimum_required(VERSION 3.15)

project(lab_3)

set(SOURCE src/main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_CXX_FLAGS "-O3 -Os -Wall -Wextra -pedantic -fopenmp -std=c++2a")
#set(CMAKE_CXX_FLAGS "-g3 -ggdb -Wall -Wextra -pedantic -fopenmp -std=c++2a")

include_directories(src/utils/include)

add_executable(main ${SOURCE})

add_subdirectory(src/utils)

target_link_libraries(main utils)

