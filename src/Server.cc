#include "Server.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "threadpool/ThreadPool.h"
#include <cassert>

Server::Server(const unsigned short port) {
  main_reactor_ = new EventLoop();
  acceptor_ = new Acceptor(main_reactor_, port);
  std::function<void(int, const InetAddress &)> cb =
      std::bind(&Server::newConnection, this, std::placeholders::_1,
                std::placeholders::_2);
  acceptor_->setNewConnectionCallback(cb);

  unsigned int size = std::thread::hardware_concurrency();
  sub_reactors_thread_pool_ = new ThreadPool(size);

  for (size_t i = 0; i < size; ++i) {
    std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
    sub_reactors_.push_back(std::move(sub_reactor));
  }
}

Server::~Server() {
  delete main_reactor_;
  delete acceptor_;
  delete sub_reactors_thread_pool_;
}

void Server::newConnection(int fd, const InetAddress &addr) {
  assert(fd != -1);
  uint64_t random = fd % sub_reactors_.size();
  ConnectorPtr conn(new Connector(fd, sub_reactors_[random].get(), addr, messagecb_, writeCompletecb_));
  connectionsMap_[fd] = conn;
  conn->setRemoveConnection(std::bind(&Server::removeConnection, this, std::placeholders::_1));
}

void Server::removeConnection(int fd){
  connectionsMap_.erase(fd);
}

void Server::Start() {
  for (size_t i = 0; i < sub_reactors_.size(); ++i) {
    // std::function<void()> sub_loop =
    //     std::bind(&EventLoop::loop, sub_reactors_[i].get());
    auto loop_function = [this, i]() { sub_reactors_[i]->loop(); };
    sub_reactors_thread_pool_->enqueue(loop_function);
  }
  main_reactor_->loop();
}