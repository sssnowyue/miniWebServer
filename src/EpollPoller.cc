#include "EpollPoller.h"
#include <cstring>
#include "Channel.h"
#include "util/Logger.h"

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

// void EpollPoller::updateChannel(Channel *channel) {
//   ChannelState status = channel->getCurState();
//   if (status == ChannelState::DELETED || status ==
//   ChannelState::READY_TO_ADD) {
//     channel->setCurState(ChannelState::ADDED);
//     EpollCtlOperate(channel, EPOLL_CTL_ADD);
//   } else {
//     if (channel->getEvents() == NoneEvent) {
//       channel->setCurState(ChannelState::DELETED);
//       EpollCtlOperate(channel, EPOLL_CTL_DEL);
//     } else {
//       EpollCtlOperate(channel, EPOLL_CTL_MOD);
//     }
//   }
// }

// void EpollPoller::deleteChannel(Channel *channel) {
//   ChannelState status = channel->getCurState();
//   if (status == ChannelState::ADDED) {
//     EpollCtlOperate(channel, EPOLL_CTL_DEL);
//   }
//   channel->setCurState(ChannelState::DELETED);
// }

void EpollPoller::updateChannel(Channel* channel) {
  ChannelState status = channel->getCurState();
  if (status == ChannelState::DELETED) {
    EpollCtlOperate(channel, EPOLL_CTL_DEL);
    if (channel->getEvents() != 0) {
      EpollCtlOperate(channel, EPOLL_CTL_ADD);
      channel->setCurState(ChannelState::ADDED);
    }
  } else if (status == ChannelState::READY_TO_ADD) {
    EpollCtlOperate(channel, EPOLL_CTL_ADD);
    channel->setCurState(ChannelState::ADDED);
  } else {
    if (channel->getEvents() == 0) {
      EpollCtlOperate(channel, EPOLL_CTL_DEL);
      channel->setCurState(ChannelState::DELETED);
    } else {
      EpollCtlOperate(channel, EPOLL_CTL_MOD);
    }
  }
}

Timestamp EpollPoller::waitPoll(int timeout,
                                std::vector<Channel*>* activeChannels) {
  // Wait for ready events
  int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);
  Timestamp nowReturn = Timestamp::now();
  if (nfds > 0) {
    // Traverse the ready events, update revents of the corresponding Channel,
    // insert these actived channel into the array
    for (int i = 0; i < nfds; ++i) {
      Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
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

void EpollPoller::EpollCtlOperate(Channel* channel, int operation) {
  epoll_event event;
  memset(&event, 0, sizeof(event));

  int fd = channel->getFd();

  event.events = channel->getEvents();
  event.data.fd = fd;
  event.data.ptr = channel;

  int ret = epoll_ctl(epfd_, operation, fd, &event);
  // errif(ret == -1, "epoll Ctl Operation error");
  if (ret < 0) {
    if (operation == EPOLL_CTL_ADD) {
      LOG_ERROR("epoll Ctl Operation EPOLL_CTL_ADD error : %d || Channal State : %d / fd : %d / Event : %d / REvent : %d", errno, channel->getCurState(), channel->getFd(), channel->getEvents(), channel->getRevents());
    } else if (operation == EPOLL_CTL_MOD) {
      LOG_ERROR("epoll Ctl Operation EPOLL_CTL_MOD error : %d", errno);
    } else {
      LOG_ERROR("epoll Ctl Operation EPOLL_CTL_DEL error : %d", errno);
    }
  }
  return;
}
