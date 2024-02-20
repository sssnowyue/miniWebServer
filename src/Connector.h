#pragma once
#include "Channel.h"
#include "callback.h"
#include "net/Socket.h"
#include "util/Buffer.h"
#include <memory>
class InetAddress;
class Connector : public std::enable_shared_from_this<Connector> {
public:
  Connector(int socketFd, EventLoop *eventLoop, const InetAddress &addr, MessageCallback &messagecb, WriteCompleteCallback &writeCompletecb);
  ~Connector();
  // void setConnectionCallback(
  //     const std::function<void(const std::shared_ptr<Connector> &)> &cb) {
  //   connetionCallback_ = cb;
  // }
  // void setMessageCallback(
  //     const std::function<void(const std::shared_ptr<Connector> &, Buffer *,
  //                              Timestamp)> &cb) {
  //   messageCallback_ = cb;
  // }
  // void setWriteCompleteCallback(
  //     const std::function<void(const std::shared_ptr<Connector> &)> &cb) {
  //   writeCompleteCallback_ = cb;
  // }
  // void setCloseCallback(
  //     const std::function<void(const std::shared_ptr<Connector> &)> &cb) {
  //   closeCallback_ = cb;
  // }

private:
  enum ConnectorState { Connecting, Connected, Disconnecting, Disconnected };

  int socketFd_;
  EventLoop *eventLoop_;
  std::unique_ptr<Socket> socket_;
  const InetAddress addr_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Buffer> inputBuffer_;
  std::unique_ptr<Buffer> outputBuffer_;
  ConnectorState state_;

  // 1. Processing functions after connection establishment/closure
  // std::function<void(const std::shared_ptr<Connector> &)> connetionCallback_;
  // 2. Processing function after receiving message
  MessageCallback messageCallback_;
  // 3. Processing function after the message is sent
  WriteCompleteCallback writeCompleteCallback_;
  // 4. Handling function after connection is closed
  // std::function<void(const std::shared_ptr<Connector> &)> closeCallback_;

  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();
public:
  void writeToBuffer_(const char *data);
};