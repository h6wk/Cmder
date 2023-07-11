/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-02 12:00:00                              *
 * @LastEditDate          : 2023-07-11 12:00:55                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "Agent.hpp"

#include <cassert>

#include "Server.hpp"
#include "Logger.hpp"


namespace Cmder {

  Server::Server()
  : mMutex()
  , mThreadPtr()
  , mConditionVariable()
  , mAgents()
  , mNotificationStatistics()
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
    assert(agent);

    std::lock_guard guard(mMutex);
    mAgents.push_back(agent);

    LOG("Agent #" << mAgents.size() << " called " << agent->getName() << " is registered");
  }

  void Server::unregister(const std::string agentName)
  {
    std::lock_guard guard(mMutex);
    for (AgentCont_t::const_iterator agentWptrIter = mAgents.begin(); agentWptrIter != mAgents.end(); /* inc. in body*/) {

      Agent::SharedPtr agent = agentWptrIter->lock();
      if ( ! agent) {
        LOG("Removing a dead reference onto an agent. No more info about!");
        agentWptrIter = mAgents.erase(agentWptrIter);
      }
      else if (agent->getName() == agentName) {
        LOG("Unregister agent called: " << agentName);
        agentWptrIter = mAgents.erase(agentWptrIter);
      }
      else {
        ++agentWptrIter;
      }
    }
  }

  uint64_t Server::statNotification(const std::string &notificationName) const
  {
    std::lock_guard guard(mMutex);
    auto it = mNotificationStatistics.find(notificationName);
    if (it != mNotificationStatistics.end()) {
      return it->second;
    }
    return 0;
  }

  void Server::run()
  {
    using namespace std::chrono_literals;

    uint64_t * tickStat = nullptr;
    {
      std::lock_guard guard(mMutex);
      mStatus = Status::Run;
      mNotificationStatistics["TICK"] = 0;
      tickStat = &(mNotificationStatistics["TICK"]);
    }

    LOG("Server-thread runs");

    while (true) {
      std::unique_lock ul(mMutex);
      if (mStatus == Status::Stop) {break;}

      mConditionVariable.wait_for(ul, 250ms);

      // Send TICK to all of the registered and alive agents
      std::for_each(std::begin(mAgents)
        , std::end(mAgents)
        , [&tickStat](const Agent::WeakPtr& agentWptr) {
          LOG("TICK");
          Agent::SharedPtr agent = agentWptr.lock();
          if (agent) {
            agent->notify("TICK");
            ++(*tickStat);
          }
        }
      );
    }

    LOG("Server-thread quits");
  }

}
