
#include "Server.hpp"
#include "Logger.hpp"

Server::Server()
: mThreadPtr()
{
}

Server::~Server()
{
  if (mThreadPtr) {
    mThreadPtr->join();
  }
}

void Server::start()
{
  mThreadPtr = std::make_unique<std::thread>(&Server::run, this);
}

void Server::stop()
{
}

void Server::run()
{
  LOG("Server runs");
}
