#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "net/InetAddress.h"
#include "net/Socket.h"
#include <functional>
class Acceptor {
public:
  Acceptor(EventLoop *eventLoop, const unsigned short port);
  ~Acceptor();
  void setCreateConnectionCallback(const std::function<void(int, const InetAddress&)> &callback) {
    newConnectionCallback_ = callback;
  }

private:
  // std::unique_ptr<Socket> acceptorSocket_;
  // std::unique_ptr<Channel> acceptorChannel_;
  EventLoop *eventLoop_;
  InetAddress *accptor_addr;
  Socket *acceptorSocket_;
  Channel *acceptorChannel_;
  // To Be Registed into channel's readCallback_ （To accept connections, and
  // distribute connections to SubEventLoop）
  void AcceptConnection();
  // Operations (distribute connections to SubEventLoop) after accepted
  std::function<void(int, const InetAddress&)> newConnectionCallback_;
};