#include "Server.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "threadpool/ThreadPool.h"
#include <cassert>
// namespace std {
// template <typename T, typename... Args>
// std::unique_ptr<T> make_unique(Args &&...args) {
//   return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
// }
// } // namespace std

Server::Server(const unsigned short port) {
  main_reactor_ = new EventLoop();
  acceptor_ = new Acceptor(main_reactor_, port);
  std::function<void(int, const InetAddress &)> cb =
      std::bind(&Server::newConnection, this, std::placeholders::_1,
                std::placeholders::_2);
  acceptor_->setNewConnectionCallback(cb);

  unsigned int size = std::thread::hardware_concurrency();
  sub_reactors_thread_pool_ = new ThreadPool(size * 2);

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
  std::unique_ptr<Connector> conn = std::make_unique<Connector>(
      fd, sub_reactors_[random].get(), addr, messagecb_, writeCompletecb_);
}

void Server::Start() {
  for (size_t i = 0; i < sub_reactors_.size(); ++i) {
    std::function<void()> sub_loop =
        std::bind(&EventLoop::loop, sub_reactors_[i].get());
    sub_reactors_thread_pool_->enqueue(std::move(sub_loop));
  }
  main_reactor_->loop();
}