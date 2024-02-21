#include "../../src/Server.h"
void onMessage(const ConnectorPtr &conn, Buffer *buf, Timestamp time) {
  std::cout << "Start retrieveAllStr" << std::endl;
  std::string msg = buf->retrieveAllStr();
  std::cout << "End retrieveAllStr" << std::endl;
  std::cout << "Message from client " << msg << std::endl;
  conn->writeToBuffer_(msg.c_str());
}
int main() {
  Server *server = new Server(8080);
  MessageCallback cb = std::bind(&onMessage, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3);
  server->setMessage(cb);
  server->Start();

  delete server;
  return 0;
}