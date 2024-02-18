#include "Acceptor.h"
#include "net/InetAddress.h"
#include "util/Logger.h"
namespace std {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace std
Acceptor::Acceptor(EventLoop *eventLoop, const unsigned short port)
    : eventLoop_(eventLoop) {
  // STEP1: socket()
  acceptorSocket_ = std::make_unique<Socket>();
  acceptorSocket_->setnonblocking();
  // Set Socket Address
  std::unique_ptr<InetAddress> accptor_addr =
      std::make_unique<InetAddress>("127.0.0.1", port);
  // STEP2: bind()
  acceptorSocket_->bind(accptor_addr.get());
  // STEP3: listen();
  acceptorSocket_->listen();

  acceptorChannel_ = std::make_unique<Channel>(
      acceptorSocket_->getFd(),
      eventLoop); // Create a channel to manage server's socket fd
  acceptorChannel_->setReadCallback(std::bind(
      &Acceptor::AcceptConnection, this)); // Set readCallback_ in channel
  acceptorChannel_
      ->enableReading(); // Set events ===> Regist server's socket fd into epoll
                         // ===> change state of channel
}

Acceptor::~Acceptor() { acceptorChannel_->disableAll(); }

void Acceptor::AcceptConnection() {
  // STEP4: accept()
  std::unique_ptr<InetAddress> client_addr = std::make_unique<InetAddress>();
  int client_fd = acceptorSocket_->accept(client_addr.get());
  if (client_fd >= 0) {
    if (newConnectionCallback_) {
      newConnectionCallback_(client_fd);
    } else {
      close(client_fd);
    }
  } else {
    LOG_ERROR("Fail to accept client");
  }
}