/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-02 12:00:00                               *
 * @LastEditDate          : 2023-07-16 23:00:21                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include <cassert>

#include <server/Server.hpp>

#include "../Logger.hpp"


namespace cmder::srv {

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

  Status Server::getStatus() const
  {
    std::lock_guard guard(mMutex);
    return mStatus;
  }

  void Server::registerAgent(const std::string& agentName, const Callback::SharedPtr& callback)
  {
    std::lock_guard guard(mMutex);
    const AgentInfo ai(agentName, callback);
    mAgents.insert(std::make_pair(agentName, ai));
    LOG("Agent #" << mAgents.size() << " called " << agentName << " was registered");
  }

  void Server::unregisterAgent(const std::string& agentName)
  {
    std::lock_guard guard(mMutex);
    unregisterAgentUnsafe(agentName);
  }

  void Server::unregisterAgentUnsafe(const std::string& agentName)
  {
    const size_t count = mAgents.erase(agentName);
    if (count) {
      LOG("Agent " << agentName << " was removed (" << count << ")");      
    }
    else {
      LOG("Cannot remove NOT registered agetn " << agentName);
    }    
  }


  uint64_t Server::statNotification(const std::string& notificationName) const
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
        , [&tickStat](const std::pair<AgentName_t, AgentInfo>& agentData) {
          LOG("TICK");

          if (agentData.second.mHasCallback) {

            // check if the callback still "valid"
            Callback::SharedPtr cb = agentData.second.mCallback.lock();
            if (cb) {
              cb->notify(INVALID_TASK_ID, CallbackMessageType_t::NOTIFICATION, "TICK");
              ++(*tickStat);
            }
            else {
              // this is a callback that no longer exists!
              //unregisterAgentUnsafe(agentName);
            }
          }
        }
      );
    }

    LOG("Server-thread quits");
  }

}
