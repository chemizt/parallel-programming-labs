cmake_minimum_required(VERSION 3.15)

project(utils)

set(SOURCE_LIB source/auxUtils.cpp source/outUtils.cpp source/randUtils.cpp)

include_directories(include)

add_library(utils STATIC ${SOURCE_LIB})