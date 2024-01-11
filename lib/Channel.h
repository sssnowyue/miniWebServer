#pragma once

#include <sys/epoll.h>
#include <functional>

/*
Data Construction of epoll_event:

typedef union epoll_data {
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t events;      // Epoll events
  epoll_data_t data;    //User data variable
} __EPOLL_PACKED;

*/

// data.ptr <---> Channel
// Manage event monitoring and processing of file descriptor
class EventLoop;
class Channel
{
public:
  EventLoop *eventLoop; // Pointer to Epoll class object
  int fd;               // File descriptor
  uint32_t events;      // Event types (EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLONESHOT)
  uint32_t revents;     // Triggered Event types when Epoll returns
  bool inEpoll;         // status of this fd : whether joined the Epoll red-black tree, dicided to use EPOLL_CTL_ADD or EPOLL_CTL_MOD operation
  std::function<void()> callback;

public:
  Channel(EventLoop *_eventLoop, int _fd);
  ~Channel();

  // Set event types
  void setEvents();
  // Set the status of fd in channel objet
  void setInEpoll();
  // Set Triggered Event types
  void setRevents(uint32_t);

  // Set CallBack function
  void setCallback(std::function<void()>);

  // run CallBack function according to channel's callback variable
  void handleEvent();

  // Epoll STEP 2 : let channel(event) to be Registered in Epoll
  void updateChannel();
};