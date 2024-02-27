#include "Acceptor.h"
#include "Channel.h"
#include "net/InetAddress.h"
#include "util/Logger.h"
Acceptor::Acceptor(EventLoop* eventLoop, const unsigned short port)
    : eventLoop_(eventLoop) {
  // STEP1: socket()
  acceptorSocket_ = new Socket();
  acceptorSocket_->setnonblocking();
  // Set Socket Address
  accptor_addr = new InetAddress("0.0.0.0", port);
  // STEP2: bind()
  acceptorSocket_->bind(accptor_addr);
  // STEP3: listen();
  acceptorSocket_->listen();

  acceptorChannel_ = new Channel(acceptorSocket_->getFd(), eventLoop);
  acceptorChannel_->setReadCallback(
      std::bind(&Acceptor::AcceptConnection, this));
  /**
  Set channel events 
    ===> Regist server's socket fd into epoll
      ===> change state of channel
  */
  acceptorChannel_->enableReading();
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
    if (createConnectionCallback_) {
      createConnectionCallback_(client_fd, client_addr);
    } else {
      close(client_fd);
    }
  } else {
    LOG_ERROR("%s | Fail to accept client", __PRETTY_FUNCTION__);
  }
}