#include "Channel.h"
#include "EventLoop.h"
#include "util/Logger.h"

Channel::Channel(int fd, EventLoop *eventloop)
    : eventLoop_(eventloop), fd_(fd), events_(0), revents_(0),
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

void Channel::handleEvent(Timestamp &tm) {
  LOG_INFO("Channel::handleEvent Revents %d BY FD %d", revents_, fd_);
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    if (closeCallback_) {
      closeCallback_();
    }
  }
  if (revents_ & EPOLLERR) {
    if (errorCallback_) {
      errorCallback_();
    }
  }
  if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (readCallback_) {
      LOG_INFO(readCallback_.target_type().name());
      readCallback_(tm);
    }
  }
  if (revents_ & EPOLLOUT) {
    if (writeCallback_) {
      writeCallback_();
    }
  }
}