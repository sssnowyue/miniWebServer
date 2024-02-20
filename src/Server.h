#pragma once
#include "Acceptor.h"
#include "Connector.h"
#include <memory>
class ThreadPool;
class InetAddress;
class Server {
public:
  Server(const unsigned short port);
  ~Server();
  void Start();
  void setMessage(const MessageCallback &cb) { messagecb_ = cb; }
  void setWriteComplete(const WriteCompleteCallback &cb) {
    writeCompletecb_ = cb;
  }

private:
  EventLoop *main_reactor_;
  Acceptor *acceptor_;
  std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

  ThreadPool *sub_reactors_thread_pool_;

  MessageCallback messagecb_;
  WriteCompleteCallback writeCompletecb_;

  void newConnection(int fd, const InetAddress &addr);
};