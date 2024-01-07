#include <iostream>
#include <string>  
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>   
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

const unsigned short port = 8080;

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
    struct sockaddr_in server_address; // sockaddr_in used for IPv4
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; // Address family type (UNIX local domain protocol family, TCP/IPv4 protocol family, TCP/IPv6 protocol family)
    server_address.sin_port = htons(port); // Port number
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Internet address

    int ret = 0;
    // STEP2: bind()
    ret = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    errif(ret == -1, "socket bind error");

    // STEP3: listen();
    ret = listen(server_socket, 5);
    errif(ret == -1, "socket listen error");
  
    while(true) {
      struct sockaddr_in client_address;
      memset(&client_address, 0, sizeof(client_address));

      // STEP4: accept()
      socklen_t address_len = sizeof(client_address);
      int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_len);

      // STEP5: read()
      char buffer[1024];
      int len = read(client_socket,buffer,sizeof(buffer));
      
      if (len > 0) {
        std::string data = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello World!</body></html>";
        //STEP6: write()
        write(client_socket, data.c_str(), data.size()); 
      }
      
      close(client_socket);
    }

    close(server_socket);
    
    return 0;
}