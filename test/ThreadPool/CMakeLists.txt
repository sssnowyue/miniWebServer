cmake_minimum_required(VERSION 3.10)
project(ModelTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(ModelTest ThreadPoolTest.cc ../../src/threadpool/ThreadPool.cc)

find_package(Threads REQUIRED)
target_link_libraries(ModelTest PRIVATE Threads::Threads)
