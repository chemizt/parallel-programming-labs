cmake_minimum_required(VERSION 3.15)

set(CMAKE_GENERATOR "Unix Makefiles")
SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

project(lab_2)

set(SOURCE_EXE src/main.cpp)

include_directories(src/utils/include)

add_executable(main.exe ${SOURCE_EXE})

add_subdirectory(src/utils)

target_link_libraries(main.exe utils)

