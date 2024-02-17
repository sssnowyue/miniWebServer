#include "InetAddress.h"

InetAddress::InetAddress() : addr_len_(sizeof(addr_)) {
  memset(&addr_, 0, addr_len_);
}
InetAddress::InetAddress(const char *ip, uint16_t port)
    : addr_len_(sizeof(addr_)) {
  memset(&addr_, 0, addr_len_);
  addr_.sin_family =
      AF_INET; // Address family type (UNIX local domain protocol family,
               // TCP/IPv4 protocol family, TCP/IPv6 protocol family)
  addr_.sin_addr.s_addr = inet_addr(ip); // Internet address
  addr_.sin_port = htons(port);          // Port number
}

InetAddress::~InetAddress() {}