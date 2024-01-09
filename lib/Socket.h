#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <fcntl.h>
#include "InetAddress.h"
#include "util.h"

class Socket
{
public:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    Socket* accept(InetAddress*);

    //set socket to be nonblock
    void setnonblocking();
};


#endif