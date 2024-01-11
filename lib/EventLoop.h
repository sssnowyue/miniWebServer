#pragma once

class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll *epfd;
    bool quit;

public:
    // Epoll STEP 1 : Create epoll instance
    EventLoop();
    ~EventLoop();

    // Epoll STEP 2 : let channel(event) to be Registered in Epoll
    void updateChannel(Channel *);
    // Epoll STEP 3 : Wait for ready events
    void waitloop();
};