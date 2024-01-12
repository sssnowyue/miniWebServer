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
    // STEP1: socket()
    Socket();
    Socket(int);
    ~Socket();

    // STEP2: bind()
    void bind(InetAddress*);
    // STEP3: listen();
    void listen();
    // STEP4: accept()
    Socket* accept(InetAddress*);

    //set socket to be nonblock
    void setnonblocking();
};

#endif