cmake_minimum_required(VERSION 3.15)

project(lab_4)

set(SOURCE src/main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_CXX_FLAGS "-O3 -Os -Wall -Wextra -pedantic -fopenmp -std=c++2a")

include_directories($ENV{MSMPI_INC} 
    $ENV{MSMPI_INC}/x64
)

find_library(
    MSMPI_LIB
    msmpi.lib
    $ENV{MSMPI_LIB64}
)

add_executable(main ${SOURCE})

target_link_libraries(main ${MSMPI_LIB})
