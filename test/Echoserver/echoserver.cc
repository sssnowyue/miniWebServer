#include "../../src/Server.h"
void process(const std::shared_ptr<Connector> &conn) {
  std::string msg = conn->read();
  std::cout << "Message from client " << msg << std::endl;
  conn->write(msg.c_str());
}
int main() {
  Server *server = new Server(8080);
  auto cb = std::bind(&process, std::placeholders::_1);
  server->setAfterRead(cb);
  server->Start();

  delete server;
  return 0;
}