#include "Epoll.h"
#include "Channel.h"

// STEP 1 : Create epoll instance
Epoll::Epoll() : epfd(-1), events(nullptr) {
  epfd = epoll_create1(0);
  errif(epfd == -1, "epoll create error");
  events = new epoll_event[MAX_EVENTS];
  memset(events, 0, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (epfd != -1) {
    close(epfd);
    epfd = -1;
  }
  delete[] events;
}

// STEP 2 : Register an event of server socket or client socket
// void Epoll::addEpFd(int fd, uint32_t op){
//     struct epoll_event ev;
//     memset(&ev, 0, sizeof(ev));
//     ev.data.fd = fd;
//     ev.events = op;

//     int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
//     errif(ret == -1, "epoll add event error");
// }
void Epoll::updateChannel(Channel *channel) {
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.ptr = channel;
  ev.events = channel->events;

  if (!channel->inEpoll) {
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, channel->fd, &ev);
    errif(ret == -1, "epoll add error");
    channel->setInEpoll();
  } else {
    int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, channel->fd, &ev);
    errif(ret == -1, "epoll modify error");
  }
}

// STEP 3 : Wait for ready events
std::vector<Channel *> Epoll::waitPoll(int timeout) {
  std::vector<Channel *> activeChannels;

  int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
  errif(nfds == -1, "epoll wait error");

  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events[i].data.ptr;
    ch->setRevents(events[i].events);
    activeChannels.push_back(ch);
  }

  return activeChannels;
}