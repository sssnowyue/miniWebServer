#pragma once
#include <sys/epoll.h>
#include <cstdint>
#include <functional>
#include "util/Timestamp.h"
/**
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

// Manage event monitoring and processing of file descriptor
data.ptr <---> Channel
*/

/**
events: 
1. EPOLLIN:         0x001       available for read(2) operations
2. EPOLLOUT:        0x004       available for write(2) operations
3. EPOLLRDHUP:      0x2000      Stream socket peer closed connection, or shut down writing half of connection.
4. EPOLLPRI:        0x002       Urgent out-of-band data available
5. EPOLLERR:        0x008       Error happened on file descriptor (Also reported for the write end of a pipe when the read end has been closed) [Not necessary to set before]
6. EPOLLHUP:        0x010       Hang up happened on file descriptor  [Not necessary to set before] (When reading from a channel such as a pipe or a stream socket, this event merely indicates that the peer closed its end of the channel.  Subsequent reads from the channel will return 0 (end of file) only after all outstanding data in the channel has been consumed.)
7. EPOLLET:         0x80000000  Enable edge-triggered mode
8. EPOLLONESHOT:    0x40000000  Set one-shot mode. File descriptor disabled after one event until re-armed
*/

class EventLoop;
enum class ChannelState { DELETED = -1, READY_TO_ADD = 0, ADDED = 1 };

class Channel {
 public:
  Channel(int fd, EventLoop* eventLoop);
  ~Channel();

  ChannelState getCurState() { return status_; }
  int getFd() const { return fd_; }
  EventLoop* getEventLoop() { return eventLoop_; }
  int getEvents() const { return events_; }
  int getRevents() const { return revents_; }

  void setCurState(ChannelState newStatus) { status_ = newStatus; }
  void enableReading();
  void disableReading();
  void enableWriting();
  void disableWriting();
  void disableAll();
  void set_revents(int revt) { revents_ = revt; }
  void setReadCallback(const std::function<void(Timestamp)>& callback) {
    readCallback_ = callback;
  }
  void setWriteCallback(const std::function<void()>& callback) {
    writeCallback_ = callback;
  }
  void setCloseCallback(const std::function<void()>& callback) {
    closeCallback_ = callback;
  }
  void setErrorCallback(const std::function<void()>& callback) {
    errorCallback_ = callback;
  }

  void handleEvent(Timestamp& tm);

 private:
  ChannelState status_;   // status of channel (-1 Deleted from Epoll, 0 Ready
                          // write into Epoll, 1 Added into Epoll)
  int fd_;                // File descriptor
  EventLoop* eventLoop_;  // Pointer to Epoll class object
  int events_;        // Event types (EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR |
                      // EPOLLHUP | EPOLLET | EPOLLONESHOT)
  uint32_t revents_;  // Triggered Event types when Epoll returns
  std::function<void(Timestamp)> readCallback_;  // Read CallBack Function
  std::function<void()> writeCallback_;          // Write CallBack Function
  std::function<void()> closeCallback_;          // Close CallBack Function
  std::function<void()> errorCallback_;          // Error CallBack Function
  const int NoneEvent = 0;
  const int ReadEvent = EPOLLIN | EPOLLPRI | EPOLLET;
  const int WriteEvent = EPOLLOUT;
};