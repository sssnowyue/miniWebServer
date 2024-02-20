add_library(server SHARED
  Acceptor.cc
  Acceptor.h
  Channel.cc
  Channel.h 
  Connector.cc
  Connector.h
  EpollPoller.cc
  EpollPoller.h
  EventLoop.cc
  EventLoop.h
  Server.cc
  Server.h
  callback.h
)

target_link_libraries(server
  net
  threadpool
  util
)

add_subdirectory(net)
add_subdirectory(threadpool) 
add_subdirectory(util)