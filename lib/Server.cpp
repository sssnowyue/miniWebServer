#include "Server.h"

#define READ_BUFFER 1024

const unsigned short port = 8080;

Server::Server(EventLoop *_loop) : eventLoop(_loop)
{
    // STEP1: socket()
    Socket *server_socket = new Socket();
    // Set Socket Address
    InetAddress *server_addr = new InetAddress("127.0.0.1", port);
    // STEP2: bind()
    server_socket->bind(server_addr);
    // STEP3: listen();
    server_socket->listen();
    server_socket->setnonblocking();

    Channel *serverChannel = new Channel(eventLoop, server_socket->fd);

    std::function<void()> cb = std::bind(&Server::newConnection, this, server_socket);
    serverChannel->setCallback(cb);

    serverChannel->setEvents();
    serverChannel->updateChannel();
}

Server::~Server()
{
}

void Server::newConnection(Socket *server_socket)
{
    // STEP4: accept()
    InetAddress *client_addr = new InetAddress();
    Socket *client_socket = server_socket->accept(client_addr);
    client_socket->setnonblocking();
    printf("new client fd %d! IP: %s Port: %d\n", client_socket->fd, inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));

    Channel *clientChannel = new Channel(eventLoop, client_socket->fd);

    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, client_socket);
    clientChannel->setCallback(cb);

    clientChannel->setEvents();
    clientChannel->updateChannel();
}

void Server::handleReadEvent(Socket *client_socket)
{
    char buf[READ_BUFFER];
    while (true)
    { // Read buf size data at a time until all is read.
        // STEP5: read()
        bzero(&buf, sizeof(buf));
        int len = read(client_socket->fd, buf, sizeof(buf));
        if (len > 0)
        {
            printf("message from client fd %d: %s\n", client_socket->fd, buf);
            // STEP6: write()
            std::string data = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello World!</body></html>";
            write(client_socket->fd, data.c_str(), data.size());
        }
        else if (len == -1 && errno == EINTR)
        { // Client interrupts normally, To continues reading
            printf("continue reading");
            continue;
        }
        else if (len == -1 && (errno == EINTR || errno == EINTR))
        { // All data has been read
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (len == 0)
        { // Client has been disconneted
            printf("EOF, client fd %d disconnected\n", client_socket->fd);
            // Close client socket (remove this socket from epoll automatically)
            close(client_socket->fd);
            break;
        }
    }
}