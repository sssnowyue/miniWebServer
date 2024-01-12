#include "EventLoop.h"
#include "Channel.h"
EventLoop::EventLoop() : epfd(nullptr), quit(false)
{
    epfd = new Epoll();
}

EventLoop::~EventLoop()
{
    delete epfd;
}

void EventLoop::updateChannel(Channel *ch)
{
    epfd->updateChannel(ch);
}

void EventLoop::waitloop()
{
    while (!quit)
    {
        std::vector<Channel *> readyChannels;
        readyChannels = epfd->waitPoll();
        for (auto it = readyChannels.begin(); it != readyChannels.end(); ++it)
        {
            (*it)->callback();
        }
    }
}