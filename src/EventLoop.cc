#include "EventLoop.h"
#include "Channel.h"
#include "EpollPoller.h"
#include <memory>
// namespace std {
// template <typename T, typename... Args>
// std::unique_ptr<T> make_unique(Args &&...args) {
//   return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
// }
// } // namespace std
const int pollTimeMs = 10000;

EventLoop::EventLoop() : epoller_(std::make_unique<EpollPoller>()) {}

EventLoop::~EventLoop() {}

void EventLoop::updateChannel(Channel *channel) const {
  epoller_->updateChannel(channel);
}

// void EventLoop::deleteChannel(Channel *channel) const {
//   epoller_->deleteChannel(channel);
// }

void EventLoop::loop() {
  while (true) {
    activeChannels_.clear();
    auto epollReturnTime_ = epoller_->waitPoll(pollTimeMs, &activeChannels_);
    for (auto activechannel : activeChannels_) {
      activechannel->handleEvent(epollReturnTime_);
    }
  }
}