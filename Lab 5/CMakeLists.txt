cmake_minimum_required(VERSION 3.15)

project(lab_5)

set(SOURCE_MAIN src/main.cpp)
set(SOURCE_WORKER src/vMMultiplySubprog.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_CXX_FLAGS "-O3 -Os -Wall -Wextra -Wno-unused-value -pedantic -fopenmp -std=c++2a")
#set(CMAKE_CXX_FLAGS "-g3 -ggdb -Wall -Wextra -Wno-unused-value -pedantic -fopenmp -std=c++2a")

include_directories(
    $ENV{I_MPI_ROOT}/intel64/include
    src/utils/include
)

find_library(
    IMPI_LIB
    impi.lib
    $ENV{I_MPI_ROOT}/intel64/lib/release
)

find_library(
    EXTERNAL
    impi.dll
    libfabric.dll
    $ENV{I_MPI_ROOT}/intel64/bin/release
    $ENV{I_MPI_ROOT}/intel64/libfabric/bin
)

add_library(
    EXTERNAL_DLL
    SHARED
    IMPORTED
)

set_property(
    TARGET EXTERNAL_DLL
    PROPERTY IMPORTED_LOCATION
    ${EXTERNAL}
)

add_subdirectory(src/utils)

add_executable(main ${SOURCE_MAIN})
add_executable(worker ${SOURCE_WORKER})

target_link_libraries(main ${IMPI_LIB} ${EXTERNAL_DLL} utils) 
target_link_libraries(worker ${IMPI_LIB} ${EXTERNAL_DLL} utils)