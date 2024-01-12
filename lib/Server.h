#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string>
#include <functional>
#include "Socket.h"
#include "Channel.h"

class EventLoop;
class Server
{
private:
    EventLoop *eventLoop;
public:
    Server(EventLoop*);
    ~Server();

    void newConnection(Socket*);
    void handleReadEvent(Socket*);
};

#endif