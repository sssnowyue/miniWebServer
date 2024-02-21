add_subdirectory(net)
add_subdirectory(threadpool) 
add_subdirectory(util)

add_library(myserver SHARED
  Acceptor.cc
  Channel.cc
  Connector.cc
  EpollPoller.cc
  EventLoop.cc
  Server.cc
)

target_include_directories(myserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(myserver
  net
  threadpool
  util
)