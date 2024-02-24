#include "Connector.h"
#include "Channel.h"
#include "net/InetAddress.h"
#include "net/Socket.h"
#include "util/Buffer.h"
#include "util/Logger.h"
#include <cstddef>
#include <cstring>
#include <iostream>
Connector::Connector(int socketFd, EventLoop *eventLoop, InetAddress addr)
    : eventLoop_(eventLoop), addr_(addr),
      socket_(std::make_unique<Socket>(socketFd)),
      channel_(std::make_unique<Channel>(socketFd, eventLoop)),
      inputBuffer_(std::make_unique<Buffer>()),
      outputBuffer_(std::make_unique<Buffer>()),
      state_(ConnectorState::Connecting) {
  socket_->setnonblocking();
  channel_->setReadCallback(
      std::bind(&Connector::handleRead, this, std::placeholders::_1));
  channel_->setWriteCallback(std::bind(&Connector::handleWrite, this));
  channel_->setCloseCallback(std::bind(&Connector::handleClose, this));
  channel_->setErrorCallback(std::bind(&Connector::handleError, this));
}

Connector::~Connector() {}

void Connector::start() {
  channel_->enableReading();
  state_ = ConnectorState::Connected;
  LOG_INFO("Connect From : %s", addr_.toIp().c_str());
}

std::string Connector::read() { return inputBuffer_->retrieveAllStr(); }

void Connector::write(const char *data) {
  size_t data_len = strlen(data);
  outputBuffer_->append(data, data_len);
  channel_->enableWriting();
  LOG_INFO("[New Test 2-23] Finished write into outbuffer | notify Epoller "
           "enableWriting");
}

void Connector::handleRead(Timestamp receiveTime) {
  int saveErrno = 0;
  ssize_t n = inputBuffer_->readFd(
      channel_->getFd(), &saveErrno); // read data from fd into inputBuffer
  if (n > 0) {                        // got data
    // messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    afterReadCallback_(shared_from_this());
  } else if (n == 0) { // client disconnect
    handleClose();
  } else { // error
    handleError();
  }
}

void Connector::handleWrite() {
  int saveErrno = 0;
  ssize_t n = outputBuffer_->writeFd(
      channel_->getFd(), &saveErrno); // write data from outputBuffer into fd
  if (n > 0) {
    // if write all data done then tell epoll to finish write; Otherwise epoll
    // will continue to write in the next time
    if (outputBuffer_->readableLen() == 0) {
      channel_->disableWriting();
      if (afterWriteCallback_) {
        afterWriteCallback_(shared_from_this());
      }
      if (state_ == Connector::Disconnecting) {
        socket_->shutdownWrite();
      }
    }
  } else {
    LOG_ERROR("Handle Write");
  }
}

void Connector::handleClose() {
  state_ = ConnectorState::Disconnected;
  channel_->disableAll();
  deleteCallback_(channel_->getFd());
  if (channel_.get() == nullptr) {
    cout << "channel_ is a NuLL" << endl;
  }
  cout << addr_.toIp() << endl;
  LOG_INFO("Disconnect From : %s", addr_.toIp().c_str());
}

void Connector::handleError() { LOG_ERROR("Handle Error"); }