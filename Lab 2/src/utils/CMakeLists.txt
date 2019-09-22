cmake_minimum_required(VERSION 3.15)

set(CMAKE_GENERATOR "Unix Makefiles")
SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

project(utils)

set(SOURCE_LIB source/auxUtils.cpp source/outUtils.cpp source/randUtils.cpp)

include_directories(include)

add_library(utils SHARED ${SOURCE_LIB})