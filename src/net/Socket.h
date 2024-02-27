#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "InetAddress.h"

class InetAddress;
class Socket {
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
  int accept(InetAddress*);

  int getFd() { return fd_; }

  // set socket to be nonblock
  void setnonblocking();

  void shutdownWrite();

 private:
  int fd_;
};