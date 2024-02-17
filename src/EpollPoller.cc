#include "EpollPoller.h"
#include "Channel.h"
#include <cstring>

EpollPoller::EpollPoller() : epfd_(-1), events_(initEpollEventListSize) {
  epfd_ = epoll_create1(0);
  errif(epfd_ == -1, "epoll create error");
}

EpollPoller::~EpollPoller() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }
}

void EpollPoller::updateChannel(Channel *channel) {
  Status status = channel->getCurStatus();
  if (status == Status::DELETED || status == Status::READY_TO_ADD) {
    channel->setCurStatus(Status::ADDED);
    EpollCtlOperate(channel, EPOLL_CTL_ADD);
  } else {
    if (channel->getEvents() == NoneEvent) {
      channel->setCurStatus(Status::DELETED);
      EpollCtlOperate(channel, EPOLL_CTL_DEL);
    } else {
      EpollCtlOperate(channel, EPOLL_CTL_MOD);
    }
  }
}

void EpollPoller::deleteChannel(Channel *channel) {
  if (channel->getCurStatus() == Status::ADDED) {
    EpollCtlOperate(channel, EPOLL_CTL_DEL);
  }
  channel->setCurStatus(Status::DELETED);
}

Timestamp EpollPoller::waitPoll(int timeout,
                                std::vector<Channel *> *activeChannels) {
  // Wait for ready events
  int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);
  Timestamp nowReturn = Timestamp::now();
  if (nfds > 0) {
    // Traverse the ready events, update revents of the corresponding Channel,
    // insert these actived channel into the array
    for (int i = 0; i < nfds; ++i) {
      Channel *ch = static_cast<Channel *>(events_[i].data.ptr);
      ch->set_revents(events_[i].events);
      activeChannels->push_back(ch);
    }
    // expand the size of array used to store ready events
    if (nfds == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  }
  return nowReturn;
}

void EpollPoller::EpollCtlOperate(Channel *channel, int operation) {
  epoll_event event;
  memset(&event, 0, sizeof(event));

  int fd = channel->getFd();

  event.events = channel->getEvents();
  event.data.fd = fd;
  event.data.ptr = channel;

  int ret = epoll_ctl(epfd_, operation, fd, &event);
  errif(ret == -1, "epoll Ctl Operation error");

  return;
}
