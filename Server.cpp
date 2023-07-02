
#include "Server.hpp"
#include "Logger.hpp"

Server::Server()
: mThreadPtr()
{
  std::lock_guard guard(mMutex);
  mStatus = Init;
}

Server::~Server()
{
  stop();
  
  if (mThreadPtr) {
    mThreadPtr->join();
  }
}

void Server::start()
{
  mThreadPtr = std::make_unique<std::thread>(&Server::run, this);
  std::lock_guard guard(mMutex);
  mStatus = Start;
}

void Server::stop()
{
  {
    std::lock_guard guard(mMutex);
    mStatus = Stop;
  }

  mConditionVariable.notify_all();
}

void Server::run()
{
  using namespace std::chrono_literals;

  {
    std::lock_guard guard(mMutex);
    mStatus = Run;
  }

  LOG("Server runs");

  while (true) {
    std::unique_lock ul(mMutex);
    if (mStatus == Stop) {break;}

    mConditionVariable.wait_for(ul, 250ms);
    LOG("TICK");
  }
}
