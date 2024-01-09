#include "Socket.h"

void vertifyFD(int fd){
    errif(fd == -1, "socket create error");
    if(fd == 0){// Close socket immediately
      struct linger tmp = {0, 1};
      setsockopt(fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }else if(fd == 1){ // Wait for unsent data to be sent, then close the socket
      struct linger tmp = {1, 1};
      setsockopt(fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
}

Socket::Socket() : fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    vertifyFD(fd);
}

Socket::Socket(int _fd) : fd(_fd){
    vertifyFD(fd);
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr){
    int ret = ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len);
    errif(ret == -1, "socket bind error");
}

void Socket::listen(){
    int ret = ::listen(fd, SOMAXCONN);
    errif(ret == -1, "socket listen error");
}

Socket* Socket::accept(InetAddress *addr){
    int client_fd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(client_fd == -1, "socket accept error");
    return new Socket(client_fd);
}

void Socket::setnonblocking(){
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}