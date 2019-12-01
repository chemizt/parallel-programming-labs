cmake_minimum_required(VERSION 3.15)

project(lab_5)

set(SOURCE_MAIN src/main.cpp)
set(SOURCE_WORKER src/worker.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_CXX_FLAGS "-O3 -Os -Wall -Wextra -Wno-unused-value -pedantic -fopenmp -std=c++2a")

include_directories($ENV{MSMPI_INC} 
    $ENV{MSMPI_INC}/x64
    src/utils/include
)

find_library(
    MSMPI_LIB
    msmpi.lib
    $ENV{MSMPI_LIB64}
)

add_subdirectory(src/utils)

add_executable(main ${SOURCE_MAIN})
add_executable(worker ${SOURCE_WORKER})

target_link_libraries(main ${MSMPI_LIB} utils) 
target_link_libraries(worker ${MSMPI_LIB} utils)