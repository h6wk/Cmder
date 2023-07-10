/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-02 12:00:00                              *
 * @LastEditDate          : 2023-07-10 15:00:51                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "Agent.hpp"

#include <cassert>

#include "Server.hpp"
#include "Logger.hpp"

using namespace Cmder;


Server::Server()
: mThreadPtr()
{
  std::lock_guard guard(mMutex);
  mStatus = Status::Init;
}

Server::~Server()
{
  stop();

  // TODO: mutex?
  if (mThreadPtr) {
    mThreadPtr->join();
  }
  LOG("Deleted");
}

void Server::start()
{
  // restart is not supported! need to destroy the object and join the thread 
  assert(mStatus == Status::Init);

  mThreadPtr = std::make_unique<std::thread>(&Server::run, this);
  std::lock_guard guard(mMutex);
  mStatus = Status::Start;
}

void Server::stop()
{
  {
    std::lock_guard guard(mMutex);
    mStatus = Status::Stop;
  }

  // Don't need to hold the lock while calling notify_all()
  mConditionVariable.notify_all();
}

Cmder::Status Server::getStatus() const
{
  std::lock_guard guard(mMutex);
  return mStatus;
}

void Server::registerAgent(Agent::SharedPtr agent)
{
  std::lock_guard guard(mMutex);
  mAgents.push_back(agent);

  LOG("Agent #" << mAgents.size() << "(" << &(*agent) << ") registered itself");
}

void Server::run()
{
  using namespace std::chrono_literals;

  {
    std::lock_guard guard(mMutex);
    mStatus = Status::Run;
  }

  LOG("Server-thread runs");

  while (true) {
    std::unique_lock ul(mMutex);
    if (mStatus == Status::Stop) {break;}

    mConditionVariable.wait_for(ul, 250ms);

    // Send TICK to all of the registered and alive agents
    std::for_each(std::begin(mAgents)
      , std::end(mAgents)
      , [](const Agent::WeakPtr& agentWptr) {
        Agent::SharedPtr agent = agentWptr.lock();
        LOG("TICK");
        if (agent) {
          agent->notify("TICK");
        }
      }
    );
  }

  LOG("Server-thread quits");
}
