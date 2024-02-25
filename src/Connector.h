#pragma once
#include "Channel.h"
#include "net/Socket.h"
#include "util/Buffer.h"
#include <memory>
class InetAddress;
class Connector : public std::enable_shared_from_this<Connector> {
public:
  Connector(int socketFd, EventLoop *eventLoop, const InetAddress& addr);
  ~Connector();

private:
  enum ConnectorState { Connecting, Connected, Disconnecting, Disconnected };
  EventLoop *eventLoop_;
  std::unique_ptr<Socket> socket_;
  InetAddress addr_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Buffer> inputBuffer_;
  std::unique_ptr<Buffer> outputBuffer_;
  ConnectorState state_;

  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  // 1. Processing function after receiving message
  std::function<void(const std::shared_ptr<Connector> &)> afterReadCallback_;
  // 2. Processing function after the message is sent
  std::function<void(const std::shared_ptr<Connector> &)> afterWriteCallback_;
  // 3. Handling function after connection is closed
  std::function<void(int)> deleteCallback_;

public:
  void start();

  std::string read();
  void write(const char *data);
  void shutdown();

  void setAfterRead(
      const std::function<void(const std::shared_ptr<Connector> &)> &cb) {
    afterReadCallback_ = cb;
  }
  void setWriteRead(
      const std::function<void(const std::shared_ptr<Connector> &)> &cb) {
    afterWriteCallback_ = cb;
  }
  void setDeleteConnection(const std::function<void(int)> &cb) {
    deleteCallback_ = cb;
  }
};