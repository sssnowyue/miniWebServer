#pragma once

#include <arpa/inet.h>
#include <string.h>

class InetAddress {
public:
  InetAddress();
  explicit InetAddress(const char *ip, uint16_t port);
  const sockaddr_in& getAddr() const { return addr; }
  const socklen_t& getAddr_len() const { return addr_len; }
  ~InetAddress();

private:
  sockaddr_in addr;
  socklen_t addr_len;
};