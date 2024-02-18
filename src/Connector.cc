#include "Connector.h"
#include "util/Buffer.h"
namespace std {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace std
Connector::Connector(int socketFd, EventLoop *eventLoop)
    : socketFd_(socketFd), eventLoop_(eventLoop),
      socket_(std::make_unique<Socket>(socketFd)),
      channel_(std::make_unique<Channel>(socketFd, eventLoop)),
      inputBuffer_(std::make_unique<Buffer>()),
      outputBuffer_(std::make_unique<Buffer>()),
      state_(ConnectorState::Connecting) {}

Connector::~Connector() {}