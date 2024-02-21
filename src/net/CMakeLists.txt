add_library(net SHARED
    InetAddress.cc
    Socket.cc
)
target_include_directories(net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
