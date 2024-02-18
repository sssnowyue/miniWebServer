#pragma once
#include "EventLoop.h"
#include "net/Socket.h"
#include <functional>
class Acceptor {
public:
  Acceptor(EventLoop *eventLoop, const unsigned short port);
  ~Acceptor();
  void setNewConnectionCallback(const std::function<void(int)> &callback) {
    newConnectionCallback_ = callback;
  }

private:
  EventLoop *eventLoop_;
  std::unique_ptr<Socket> acceptorSocket_;
  std::unique_ptr<Channel> acceptorChannel_;
  // To Be Registed into channel's readCallback_ 
  // To accept connections, and distribute connections to SubEventLoop
  void AcceptConnection();
  // Operations after accepted
  std::function<void(int)> newConnectionCallback_;
};