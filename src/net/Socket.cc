#include "Socket.h"
#include <sys/socket.h>
#include "../util/Errif.h"
#include "../util/Logger.h"

void vertifyFD(int fd) {
  errif(fd == -1, "socket create error");
  if (fd == 0) {  // Close socket immediately
    struct linger tmp = {0, 1};
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
  } else if (fd ==
             1) {  // Wait for unsent data to be sent, then close the socket
    struct linger tmp = {1, 1};
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
  }
}

Socket::Socket() : fd_(-1) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  vertifyFD(fd_);
}

Socket::Socket(int _fd) : fd_(_fd) {
  vertifyFD(fd_);
}

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Socket::bind(InetAddress* addr) {
  int ret = ::bind(fd_, (sockaddr*)&addr->getAddr(), sizeof(addr->getAddr()));
  errif(ret == -1, "socket bind error");
}

void Socket::listen() {
  int ret = ::listen(fd_, SOMAXCONN);
  errif(ret == -1, "socket listen error");
}

int Socket::accept(InetAddress* addr) {
  int client_fd = ::accept(fd_, (sockaddr*)&addr->getAddr(),
                           (socklen_t*)&addr->getAddr_len());
  errif(client_fd == -1, "socket accept error");
  return client_fd;
}

void Socket::setnonblocking() {
  int flags = fcntl(fd_, F_GETFL, 0);
  fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
}

void Socket::shutdownWrite() {
  if (::shutdown(fd_, SHUT_WR) < 0) {
    LOG_ERROR("shutdownWrite error");
  }
}