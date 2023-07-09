/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-02 12:00:00                              *
 * @LastEditDate          : 2023-07-09 23:19:58                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "Agent.hpp"
#include "Server.hpp"
#include "Logger.hpp"

using namespace Cmder;


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

Server::Status Server::getStatus() const
{
  std::lock_guard guard(mMutex);
  return mStatus;
}

void Server::registerAgent(Agent::SharedPtr agent)
{
  std::lock_guard guard(mMutex);
  mAgents.push_back(agent);
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

    // Send TICK to all of the registered and alive agents
    std::for_each(std::begin(mAgents)
      , std::end(mAgents)
      , [](const Agent::WeakPtr& agentWptr) {
        Agent::SharedPtr agent = agentWptr.lock();
        if (agent) {
          LOG("TICK");
          agent->notify("TICK");
        }
      }
    );
  }

  LOG("Server quits");
}
