#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
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
class Epoll;
class Channel
{
public:
    Epoll *ep;          // Pointer to Epoll class object
    int fd;             // File descriptor
    uint32_t events;    // Event types (EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLONESHOT)
    uint32_t revents;   // Triggered Event types when Epoll returns
    bool inEpoll;       // status of this fd : whether joined the Epoll red-black tree, dicided to use EPOLL_CTL_ADD or EPOLL_CTL_MOD operation
public:
    Channel(Epoll *_ep, int _fd);
    ~Channel();

    // set event types
    void setEvents();
    // set the status of fd in channel objet
    void setInEpoll();
    // set Triggered Event types
    void setRevents(uint32_t);
};


#endif