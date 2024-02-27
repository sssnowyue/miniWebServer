#pragma once
#include <functional>
#include <memory>
#include "Connector.h"
class ThreadPool;
class InetAddress;
class Acceptor;
class Server {
 public:
  Server(const unsigned short port);
  ~Server();
  void Start();
  // set callback function used to be run after receive message from client
  void setAfterRead(
      const std::function<void(const std::shared_ptr<Connector>&)>& cb) {
    afterReadCallback_ = cb;
  }
  // set callback function used to be run after send message into client
  void setAfterWrite(
      const std::function<void(const std::shared_ptr<Connector>&)>& cb) {
    afterWriteCallback_ = cb;
  }

 private:
  /**
  Main Reactor:
    To serve Accpetor
  */
  EventLoop* main_reactor_;
  /**
  Acceptor:
    1. Listen,
    2. Accept new clients,
    3. Assign them to SubReactors
  */
  Acceptor* acceptor_;

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
  std::unordered_map<int, std::shared_ptr<Connector>> connectionsMap_;

  /**
  Thread Pool:
    To Run SubReactors
  */
  ThreadPool* sub_reactors_thread_pool_;

  // Process after receive message from client
  std::function<void(const std::shared_ptr<Connector>&)> afterReadCallback_;
  // Process after send message into client
  std::function<void(const std::shared_ptr<Connector>&)> afterWriteCallback_;

  // Part of Process after Acceptor accepted new client : Assign them to SubReactors
  void createConnection(int fd, const InetAddress& addr);
  // Part of Process after client disconnect : Destroy Connector
  void deleteConnection(int fd);
};