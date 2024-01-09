#include <iostream>
#include <string>  
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>   
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "lib/util.h"
#include "lib/InetAddress.h"
#include "lib/Socket.h"
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

const unsigned short port = 8080;

int main() {
    // STEP1: socket()
    Socket *server_socket = new Socket();
    // Set Socket Address
    InetAddress *server_addr = new InetAddress("127.0.0.1", port);
    // STEP2: bind()
    server_socket->bind(server_addr);
    // STEP3: listen();
    server_socket->listen();
    // I/O multiplSocketexing Epoll - STEP 1 : Create epoll instance
    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    // I/O multiplexing Epoll - STEP 2 : Register an event of server socket
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_socket->fd;
    server_socket->setnonblocking();
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket->fd, &ev);

    struct epoll_event events[MAX_EVENTS];
    bzero(&events, sizeof(events));
  
    while(true) {
      // I/O multiplexing Epoll - STEP 3 : Wait for events
      int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
      errif(nfds == -1, "epoll wait error");

      for(int i = 0; i < nfds; ++i){
        // New client connetion
        if(events[i].data.fd == server_socket->fd){
          // STEP4: accept()
          InetAddress *client_addr = new InetAddress();
          Socket *client_socket = server_socket->accept(client_addr);
          printf("new client fd %d! IP: %s Port: %d\n", client_socket->fd, inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));

          // I/O multiplexing Epoll - STEP 2 : Register an event of connected client socket
          bzero(&ev, sizeof(ev));
          ev.data.fd = client_socket->fd;
          ev.events = EPOLLIN | EPOLLET;
          client_socket->setnonblocking();
          epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket->fd, &ev);
        // Ready to Read
        }else if(events[i].events & EPOLLIN){
          char buf[READ_BUFFER];
          while(true){ // Read buf size data at a time until all is read.
            // STEP5: read()
            bzero(&buf, sizeof(buf));
            int len = read(events[i].data.fd, buf, sizeof(buf));
            if(len > 0){
              printf("message from client fd %d: %s\n", events[i].data.fd, buf);
              //STEP6: write()
              std::string data = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello World!</body></html>";
              write(events[i].data.fd, data.c_str(), data.size());
            }else if(len == -1 && errno == EINTR){ // Client interrupts normally, To continues reading
              printf("continue reading");
              continue;
            }else if(len == -1 && (errno == EINTR || errno == EINTR)){ // All data has been read
              printf("finish reading once, errno: %d\n", errno);
              break;
            }else if(len == 0){ // Client has been disconneted
              printf("EOF, client fd %d disconnected\n", events[i].data.fd);
              //Close client socket (remove this socket from epoll automatically)
              close(events[i].data.fd);
              break;
            }
          }
        }else{
          printf("other condition to be catch in later version\n");
        }
      }
    }
    // close(server_socket);
    return 0;
}