#pragma once
#include "Channel.h"
#include "net/Socket.h"
#include "util/Buffer.h"
#include <atomic>
#include <memory>
class Connector {
public:
  Connector(int socketFd, EventLoop *eventLoop);
  ~Connector();

private:
  enum ConnectorState { Connecting, Connected, Disconnecting, Disconnected };
  int socketFd_;
  EventLoop *eventLoop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Buffer> inputBuffer_;
  std::unique_ptr<Buffer> outputBuffer_;
  ConnectorState state_;
};