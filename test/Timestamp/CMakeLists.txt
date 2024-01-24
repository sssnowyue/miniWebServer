cmake_minimum_required(VERSION 3.12)
project(ModelTest)
set(CMAKE_CXX_STANDARD 20)
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

file(GLOB_RECURSE SOURCES *.cc ../../src/util/*.cc)

add_executable(${PROJECT_NAME} ${SOURCES})