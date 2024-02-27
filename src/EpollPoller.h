#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

class Timestamp;
class Channel;
// I/O multiplSocketexing Epoll
class EpollPoller {
 public:
  // STEP 1 : Create epoll instance
  EpollPoller();
  ~EpollPoller();
  // STEP 2 : manage(add/modify/delete) an event of server socket or client socket
  void updateChannel(Channel* channel);
  // STEP 3 : Wait for ready events
  Timestamp waitPoll(int timeout, std::vector<Channel*>* activeChannels);

 private:
  int epfd_;
  // inital size of events_
  static const int initEpollEventListSize = 16;
  // used to store ready events
  std::vector<epoll_event> events_;
  // Operations of Epoll(epoll_ctl add/mod/del) used for Function updateChannel
  void EpollCtlOperate(Channel* channel, int operation);
};