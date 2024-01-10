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
#include "lib/Epoll.h"

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
    Epoll *ep = new Epoll();
    // I/O multiplexing Epoll - STEP 2 : Register an event of server socket
    ep->addEpFd(server_socket->fd, EPOLLIN | EPOLLET);
  
    while(true) {
      // I/O multiplexing Epoll - STEP 3 : Wait for ready events
      std::vector<epoll_event> activeEvents = ep->poll();

      int nfds = activeEvents.size();
      for(int i = 0; i < nfds; ++i){
        // New client connetion
        if(activeEvents[i].data.fd == server_socket->fd){
          // STEP4: accept()
          InetAddress *client_addr = new InetAddress();
          Socket *client_socket = server_socket->accept(client_addr);
          printf("new client fd %d! IP: %s Port: %d\n", client_socket->fd, inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));

          // I/O multiplexing Epoll - STEP 2 : Register an event of connected client socket
          ep->addEpFd(client_socket->fd, EPOLLIN | EPOLLET);

        // Ready to Read
        }else if(activeEvents[i].events & EPOLLIN){
          char buf[READ_BUFFER];
          while(true){ // Read buf size data at a time until all is read.
            // STEP5: read()
            bzero(&buf, sizeof(buf));
            int len = read(activeEvents[i].data.fd, buf, sizeof(buf));
            if(len > 0){
              printf("message from client fd %d: %s\n", activeEvents[i].data.fd, buf);
              //STEP6: write()
              std::string data = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello World!</body></html>";
              write(activeEvents[i].data.fd, data.c_str(), data.size());
            }else if(len == -1 && errno == EINTR){ // Client interrupts normally, To continues reading
              printf("continue reading");
              continue;
            }else if(len == -1 && (errno == EINTR || errno == EINTR)){ // All data has been read
              printf("finish reading once, errno: %d\n", errno);
              break;
            }else if(len == 0){ // Client has been disconneted
              printf("EOF, client fd %d disconnected\n", activeEvents[i].data.fd);
              //Close client socket (remove this socket from epoll automatically)
              close(activeEvents[i].data.fd);
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