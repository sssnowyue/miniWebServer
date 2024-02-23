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
  /**
  Main Reactor:
    To serve Accpetor
  */
  EventLoop *main_reactor_;
  /**
  Acceptor:
    1. Listen,
    2. Accept new clients,
    3. Assign them to SubReactors
  */
  Acceptor *acceptor_;

  /**
  SubReactors:
    To serve Connectors
  */
  std::vector<std::unique_ptr<EventLoop>> sub_reactors_;
  /**
  Connectors:
    1. Read from client,
    2. Assign logical processing tasks to workers,
    3. Write back to client
  */
  std::unordered_map<int, ConnectorPtr> connectionsMap_;

  ThreadPool *sub_reactors_thread_pool_;

  MessageCallback messagecb_;
  WriteCompleteCallback writeCompletecb_;

  void createConnection(int fd, const InetAddress &addr);
  void deleteConnection(int fd);
};