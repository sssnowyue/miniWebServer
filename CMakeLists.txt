cmake_minimum_required(VERSION 3.12)
project(Echo_Server)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_SUBDIRECTORY(src)

# 添加可执行文件
add_executable(echoserver echoserver.cc)

find_package(Threads REQUIRED)
target_link_libraries(echoserver server Threads::Threads)

