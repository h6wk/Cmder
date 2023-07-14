/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-02 12:00:00                              *
 * @LastEditDate          : 2023-07-13 23:57:13                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef A694C050_D5F4_4037_BF8A_42619171DEE0
#define A694C050_D5F4_4037_BF8A_42619171DEE0

#include <agent/Agent.hpp>
#include <IControllableThread.hpp>
#include <IStatProvider.hpp>

#include "../../Logger.hpp"

#include <condition_variable>
#include <map>
#include <memory>
#include <thread>

namespace cmder::srv {

  using namespace cmder::agent;

  /**
   * Entity that accepts the task does the job and delivers the result to the
   * agent. It can work in sync or async mode, that depends on the type of 
   * the task.
   */
  class Server : public IControllableThread, public IStatProvider {
  public:
    
    Server();
    virtual ~Server();

    //////////////////////////////////////////////////////////////////////////////////
    // IControllableThread abstract interface implementations:                      //
    //////////////////////////////////////////////////////////////////////////////////
    void start() override;                                                          //
    void stop() override;                                                           //
    Status getStatus() const override;                                              //
    //////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////////////
    // IStatProvider abstract interface implementations:                            //
    //////////////////////////////////////////////////////////////////////////////////
    uint64_t statNotification(const std::string& notificationName) const override;  //
    //////////////////////////////////////////////////////////////////////////////////


    /// @brief Registers an agent into the server (for notifications)
    void registerAgent(const std::string& agentName, const Callback::SharedPtr& callback);

    /// @brief Temp. solution to remove an agent. Not completely safe to identify the agent with a generated name.
    /// @param agentName to remove
    void unregisterAgent(const std::string& agentName);

  private:
    void unregisterAgentUnsafe(const std::string& agentName);

    void run();

    mutable std::mutex mMutex;                  //< Protect data containers against data race

    std::unique_ptr<std::thread> mThreadPtr;    //< Do internal calculations. Send notifications

    std::condition_variable mConditionVariable; //< For the control of the thread

    Status mStatus;                             //< The status of mThreadPtr
    

    using AgentName_t = std::string;
    struct AgentInfo {
      explicit AgentInfo(const AgentName_t& n, const Callback::SharedPtr& sp)
      : mAgentName(n)
      , mHasCallback(sp ? true : false)
      , mCallback(sp)
      {
        LOG("Registration info for " << mAgentName << " hasCallbak=" << mHasCallback);
      }
      
      AgentName_t mAgentName;
      bool mHasCallback;
      Callback::WeakPtr mCallback;
    };

    using AgentCont_t = std::map<AgentName_t, AgentInfo>;
    AgentCont_t mAgents;                        //< List of agents that registered itself. The agent lifetime is not
                                                //< known. Need to lock to test the existence of it

    std::map<std::string, uint64_t> mNotificationStatistics;  //< Nr. of notifications sent of a given type
  };

}

#endif /* A694C050_D5F4_4037_BF8A_42619171DEE0 */
