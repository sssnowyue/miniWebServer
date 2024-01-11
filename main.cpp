#include "lib/EventLoop.h"
#include "lib/Server.h"

int main()
{
  EventLoop *eventLoop = new EventLoop();
  Server *server = new Server(eventLoop);
  eventLoop->waitloop();
  return 0;
}