#include "Channel.h"

Channel::Channel(EventLoop *_eventLoop, int _fd) : eventLoop(_eventLoop), fd(_fd), events(0), revents(0), inEpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::setEvents()
{
    events = EPOLLIN | EPOLLET;
}

void Channel::setInEpoll()
{
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev)
{
    revents = _ev;
}

void Channel::updateChannel()
{
    eventLoop->updateChannel(this);
}

void Channel::setCallback(std::function<void()> _cb)
{
    callback = _cb;
}

// void Channel::handleEvent()
// {
//     callback();
// }