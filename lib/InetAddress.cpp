#include "InetAddress.h"

InetAddress::InetAddress() : addr_len(sizeof(addr)){
    memset(&addr, 0, addr_len);
}
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)){
    memset(&addr, 0, addr_len);
    addr.sin_family = AF_INET; // Address family type (UNIX local domain protocol family, TCP/IPv4 protocol family, TCP/IPv6 protocol family)
    addr.sin_addr.s_addr = inet_addr(ip); // Internet address
    addr.sin_port = htons(port); // Port number
}

InetAddress::~InetAddress(){}