#include <iostream>
#include <string>  
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>   
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "lib/util.h"
#include "lib/InetAddress.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

const unsigned short port = 8080;

void setnonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    // STEP1: socket()
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    errif(server_socket == -1, "socket create error");
    if(server_socket == 0){// Close socket immediately
      struct linger tmp = {0, 1};
      setsockopt(server_socket, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }else if(server_socket == 1){ // Wait for unsent data to be sent, then close the socket
      struct linger tmp = {1, 1};
      setsockopt(server_socket, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    // Set Socket Address
    InetAddress *server_addr = new InetAddress("127.0.0.1", port);
    
    int ret = 0;
    // STEP2: bind()
    ret = bind(server_socket, (struct sockaddr*) &server_addr->addr, server_addr->addr_len);
    errif(ret == -1, "socket bind error");

    // STEP3: listen();
    ret = listen(server_socket, 5);
    errif(ret == -1, "socket listen error");

    // I/O multiplexing Epoll - STEP 1 : Create epoll instance
    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    // I/O multiplexing Epoll - STEP 2 : Register an event of server socket
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_socket;
    setnonblocking(server_socket);
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &ev);

    struct epoll_event events[MAX_EVENTS];
    bzero(&events, sizeof(events));
  
    while(true) {
      // I/O multiplexing Epoll - STEP 3 : Wait for events
      int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
      errif(nfds == -1, "epoll wait error");

      for(int i = 0; i < nfds; ++i){
        // New client connetion
        if(events[i].data.fd == server_socket){
          // STEP4: accept()
          struct sockaddr_in client_address;
          memset(&client_address, 0, sizeof(client_address));
          socklen_t address_len = sizeof(client_address);
          int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_len);
          errif(client_socket == -1, "socket accept error");
          printf("new client fd %d! IP: %s Port: %d\n", client_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

          // I/O multiplexing Epoll - STEP 2 : Register an event of connected client socket
          bzero(&ev, sizeof(ev));
          ev.data.fd = client_socket;
          ev.events = EPOLLIN | EPOLLET;
          setnonblocking(client_socket);
          epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev);
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
    close(server_socket);
    return 0;
}