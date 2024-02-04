#pragma once
#include "../util/Errif.h"
#include "InetAddress.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

class InetAddress;
class Socket{
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
private:
    int fd;
};