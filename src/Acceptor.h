#pragma once
#include <functional>
#include "Channel.h"
#include "EventLoop.h"
#include "net/InetAddress.h"
#include "net/Socket.h"
class Acceptor {
 public:
  Acceptor(EventLoop* eventLoop, const unsigned short port);
  ~Acceptor();
  // Set Part of Process after Acceptor accepted new client : Assign them to SubReactors
  void setCreateConnectionCallback(
      const std::function<void(int, const InetAddress&)>& callback) {
    createConnectionCallback_ = callback;
  }

 private:
  EventLoop* eventLoop_;
  InetAddress* accptor_addr;
  Socket* acceptorSocket_;
  Channel* acceptorChannel_;
  // Part of Process after Acceptor accepted new client : Assign them to SubReactors
  std::function<void(int, const InetAddress&)> createConnectionCallback_;
  /**
  Server Channel's readCallback_ :
    1. Accept Clients
    2. Assign them to SubReactors
  */
  void AcceptConnection();
};