#pragma once
#include "../util/Errif.h"
#include "InetAddress.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

class InetAddress;
class Socket {
public:
  // STEP1: socket()
  Socket();
  Socket(int);
  ~Socket();

  // STEP2: bind()
  void bind(InetAddress *);
  // STEP3: listen();
  void listen();
  // STEP4: accept()
  int accept(InetAddress *);

  int getFd(){ return fd_;}

  // set socket to be nonblock
  void setnonblocking();

private:
  int fd_;
};