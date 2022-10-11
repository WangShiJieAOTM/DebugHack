cmake_minimum_required(VERSION 3.22)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE Debug)

project(demo)

add_executable(demo demo.cpp demo.h)