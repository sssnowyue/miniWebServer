#include "Acceptor.h"
#include "Channel.h"
#include "net/InetAddress.h"
#include "util/Logger.h"
Acceptor::Acceptor(EventLoop *eventLoop, const unsigned short port)
    : eventLoop_(eventLoop) {
  // STEP1: socket()
  acceptorSocket_ = new Socket();
  acceptorSocket_->setnonblocking();
  // Set Socket Address
  accptor_addr = new InetAddress("127.0.0.1", port);
  // STEP2: bind()
  acceptorSocket_->bind(accptor_addr);
  // STEP3: listen();
  acceptorSocket_->listen();

  acceptorChannel_ =
      new Channel(acceptorSocket_->getFd(),
                  eventLoop); // Create a channel to manage server's socket fd
  acceptorChannel_->setReadCallback(std::bind(
      &Acceptor::AcceptConnection, this)); // Set readCallback_ in channel
  acceptorChannel_
      ->enableReading(); // Set events ===> Regist server's socket fd into epoll
                         // ===> change state of channel
}

Acceptor::~Acceptor() {
  acceptorChannel_->disableAll();
  delete acceptorSocket_;
  delete accptor_addr;
  delete acceptorChannel_;
}

void Acceptor::AcceptConnection() {
  // STEP4: accept()
  InetAddress client_addr;
  int client_fd = acceptorSocket_->accept(&client_addr);
  if (client_fd >= 0) {
    if (newConnectionCallback_) {
      newConnectionCallback_(client_fd, client_addr);
    } else {
      close(client_fd);
    }
  } else {
    LOG_ERROR("Fail to accept client");
  }
}