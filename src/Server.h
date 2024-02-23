#pragma once
#include "Connector.h"
#include <memory>
class ThreadPool;
class InetAddress;
class Acceptor;
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

  std::unordered_map<int, ConnectorPtr> connectionsMap_;

  ThreadPool *sub_reactors_thread_pool_;

  MessageCallback messagecb_;
  WriteCompleteCallback writeCompletecb_;
  void removeConnection(int);

  void newConnection(int fd, const InetAddress &addr);
};