#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress {
 public:
  InetAddress();
  explicit InetAddress(const char* ip, uint16_t port);
  const sockaddr_in& getAddr() const { return addr_; }
  const socklen_t& getAddr_len() const { return addr_len_; }
  std::string toIp() const;
  ~InetAddress();

 private:
  sockaddr_in addr_;
  socklen_t addr_len_;
};