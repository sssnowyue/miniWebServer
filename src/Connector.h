#pragma once
#include <memory>
#include "Channel.h"
#include "net/InetAddress.h"
class Socket;
class Buffer;
class Connector : public std::enable_shared_from_this<Connector> {
 public:
  Connector(int socketFd, EventLoop* eventLoop, const InetAddress& addr);
  ~Connector();
  /**
  Start connector: 
    1. update client channal for reading
    2. set connector state
    3. log conneted client's info
  */
  void start();
  /**
  Connection closed by server:
    shutdown(client_fd, SHUT_WR)
  */
  void shutdown();
  // Read data from inputBuffer For later logical processing
  std::string read();
  // 1. Write data from logical processing into outBuffer; 2. update client channal for writing
  void write(const char* data);

  // Set Processing function after receiving message
  void setAfterRead(
      const std::function<void(const std::shared_ptr<Connector>&)>& cb) {
    afterReadCallback_ = cb;
  }
  // Set Processing function after the message is sent
  void setAfterWrite(
      const std::function<void(const std::shared_ptr<Connector>&)>& cb) {
    afterWriteCallback_ = cb;
  }
  // Set Handling function after connection is closed
  void setDeleteConnection(const std::function<void(int)>& cb) {
    deleteCallback_ = cb;
  }

 private:
  enum ConnectorState { Connecting, Connected, Disconnecting, Disconnected };
  EventLoop* eventLoop_;
  std::unique_ptr<Socket> socket_;
  const InetAddress addr_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Buffer> inputBuffer_;
  std::unique_ptr<Buffer> outputBuffer_;
  ConnectorState state_;

  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  // Processing function after receiving message
  std::function<void(const std::shared_ptr<Connector>&)> afterReadCallback_;
  // Processing function after the message is sent
  std::function<void(const std::shared_ptr<Connector>&)> afterWriteCallback_;
  // Handling function after connection is closed
  std::function<void(int)> deleteCallback_;
};