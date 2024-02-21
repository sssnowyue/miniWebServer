add_library(threadpool SHARED
    ThreadPool.cc
)

target_include_directories(threadpool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
