#include "EventLoop.h"
#include <memory>
#include "Channel.h"
#include "EpollPoller.h"
const int pollTimeMs = 10000;

EventLoop::EventLoop() : epoller_(std::make_unique<EpollPoller>()) {}

EventLoop::~EventLoop() {}

void EventLoop::updateChannel(Channel* channel) const {
  epoller_->updateChannel(channel);
}

void EventLoop::loop() {
  while (true) {
    activeChannels_.clear();
    auto epollReturnTime_ = epoller_->waitPoll(pollTimeMs, &activeChannels_);
    for (auto activechannel : activeChannels_) {
      activechannel->handleEvent(epollReturnTime_);
    }
  }
}