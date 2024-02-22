#include "Connector.h"
#include "net/InetAddress.h"
#include "util/Buffer.h"
#include "util/Logger.h"
#include <cassert>
#include <cstring>
Connector::Connector(int socketFd, EventLoop *eventLoop,
                     const InetAddress &addr, MessageCallback &messagecb,
                     WriteCompleteCallback &writeCompletecb)
    : socketFd_(socketFd), eventLoop_(eventLoop), addr_(addr),
      messageCallback_(messagecb), writeCompleteCallback_(writeCompletecb),
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

  channel_->enableReading();
  state_ = ConnectorState::Connected;
  LOG_INFO("Connect From : %s", addr_.toIp().c_str());
}

Connector::~Connector() {}

void Connector::handleRead(Timestamp receiveTime) {
  LOG_INFO("New Test - Connector HandleRead: Begin to RUN handleRead");
  int saveErrno = 0;
  ssize_t n = inputBuffer_->readFd(
      socketFd_, &saveErrno); // read data from fd into inputBuffer
  LOG_INFO("New Test - Connector HandleRead: I have Received data with length "
           "of %d from Client",
           n);
  if (n > 0) { // got data
    if (shared_from_this().get() == nullptr) {
      LOG_ERROR("shared_from_this is nullptr!");
      return;
    }
    if (inputBuffer_.get() == nullptr) {
      LOG_ERROR("inputBuffer_ is nullptr!");
      return;
    }
    messageCallback_(shared_from_this(), inputBuffer_.get(), receiveTime);
  } else if (n == 0) { // client disconnect
    handleClose();
  } else { // error
    handleError();
  }
}

void Connector::writeToBuffer_(const char *data) {
  size_t data_len = strlen(data);
  outputBuffer_->append(data, data_len);
  channel_->enableReading();
}

void Connector::handleWrite() {
  int saveErrno = 0;
  ssize_t n = outputBuffer_->writeFd(
      socketFd_, &saveErrno); // write data from outputBuffer into fd
  if (n > 0) {
    // if write all data done then tell epoll to finish write; Otherwise epoll
    // will continue to write in the next time
    if (outputBuffer_->readableLen() == 0) {
      channel_->disableWriting();
      if (writeCompleteCallback_) {
        writeCompleteCallback_(shared_from_this());
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
  LOG_INFO("Disconnect From : %s", addr_.toIp().c_str());
}

void Connector::handleError() { LOG_ERROR("Handle Error"); }