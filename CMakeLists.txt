cmake_minimum_required(VERSION 3.12)
project(Echo_Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_SUBDIRECTORY(src)

add_executable(echoserver ./test/Echoserver/echoserver.cc)

find_package(Threads REQUIRED)
target_link_libraries(echoserver myserver Threads::Threads)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")