#pragma once
#include "Channel.h"
#include <vector>
class EpollPoller;
class EventLoop {
public:
  // Epoll STEP 1 : Create epoll instance
  EventLoop();
  ~EventLoop();
  // STEP 2 - 1 : manage(add/modify/delete) an event of server socket or client
  // socket
  void updateChannel(Channel *channel) const;
  // STEP 2 - 2 : manage(delete) an event of server socket or client
  // socket
  void deleteChannel(Channel *channel) const;
  // STEP 3 : Wait for ready events
  void loop();

private:
  std::unique_ptr<EpollPoller> epoller_;
  std::vector<Channel*> activeChannels_;
};