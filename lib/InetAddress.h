#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <arpa/inet.h>
#include <string.h>

class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};

#endif