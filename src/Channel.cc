#include "Channel.h"
#include "EventLoop.h"

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
  if (revents_ & EPOLLHUP) {
    if (closeCallback_) {
      closeCallback_();
    }
  } else if (revents_ & EPOLLERR) {
    if (errorCallback_) {
      errorCallback_();
    }
  } else if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (readCallback_) {
      readCallback_(tm);
    }
  } else if (revents_ & EPOLLOUT) {
    if (writeCallback_) {
      writeCallback_();
    }
  }
}