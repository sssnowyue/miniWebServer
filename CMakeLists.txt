cmake_minimum_required(VERSION 3.10)

project(miniWebServer)

set(CMAKE_CXX_STANDARD 11)

file(GLOB LIB_SOURCES ${PROJECT_SOURCE_DIR}/lib/*.cpp) 

add_executable(miniWebServer main.cpp ${LIB_SOURCES})