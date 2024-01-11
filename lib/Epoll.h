#pragma once

#include <sys/epoll.h>
#include <vector>
#include "util.h"
#include <string.h>
#include <unistd.h>
// #include "Channel.h"

#define MAX_EVENTS 1000

class Channel;
// I/O multiplSocketexing Epoll
class Epoll
{
private:
    int epfd;
    struct epoll_event *events;

public:
    // STEP 1 : Create epoll instance
    Epoll();
    ~Epoll();
    // STEP 2 : Register an event of server socket or client socket
    // void addEpFd(int fd, uint32_t op);
    void updateChannel(Channel *channel);
    // STEP 3 : Wait for ready events
    std::vector<Channel *> waitPoll(int timeout = -1);
};