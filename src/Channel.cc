#include "Channel.h"
#include "EventLoop.h"
#include "util/Logger.h"

Channel::Channel(int fd, EventLoop* eventloop)
    : eventLoop_(eventloop),
      fd_(fd),
      events_(0),
      revents_(0),
      status_(ChannelState::READY_TO_ADD) {}

Channel::~Channel() {}

void Channel::enableReading() {
  events_ |= ReadEvent;
  eventLoop_->updateChannel(this);
}
void Channel::disableReading() {
  events_ &= ~ReadEvent;
  eventLoop_->updateChannel(this);
}
void Channel::enableWriting() {
  events_ |= WriteEvent;
  eventLoop_->updateChannel(this);
}
void Channel::disableWriting() {
  events_ &= ~WriteEvent;
  eventLoop_->updateChannel(this);
}
void Channel::disableAll() {
  events_ = NoneEvent;
  eventLoop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp& tm) {
  // LOG_INFO("Channel (FD %d) handle Revent %d", fd_, revents_);
  if ((revents_ & EPOLLHUP)) {
    if (closeCallback_) {
      LOG_INFO("closeCallback_");
      closeCallback_();
    }
  } else if (revents_ & EPOLLERR) {
    if (errorCallback_) {
      LOG_INFO("errorCallback_");
      errorCallback_();
    }
  } else if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (readCallback_) {
      LOG_INFO("readCallback_");
      readCallback_(tm);
    }
  } else if (revents_ & EPOLLOUT) {
    if (writeCallback_) {
      LOG_INFO("writeCallback_");
      writeCallback_();
    }
  }
}