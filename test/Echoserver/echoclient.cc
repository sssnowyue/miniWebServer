#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char const *hello = "GET / HTTP/1.1\r\nHost: yourhostname.com\r\n\r\n";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // 服务器地址
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // 发送请求
    send(sock , hello , strlen(hello) , 0);
    send(sock , hello , strlen(hello) , 0);
    send(sock , hello , strlen(hello) , 0);
    std::cout << "Request sent" << std::endl;

    // 接收服务器响应
    valread = read( sock , buffer, 1024);
    std::cout << buffer << std::endl;

    return 0;
}
