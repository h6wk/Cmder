/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-02 12:00:00                              *
 * @LastEditDate          : 2023-07-10 23:54:22                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Agent.hpp"
#include "IControllableThread.hpp"

#include <condition_variable>
#include <memory>
#include <thread>

namespace Cmder {

  class Server : public IControllableThread {
  public:
    
    Server();
    virtual ~Server();

    // IControllableThread abstract interfaces:
    void start() override;
    void stop() override;
    Cmder::Status getStatus() const override;

    /// @brief Registers an agent into the server (for notifications)
    /// @param agent Shared pointer to the agent
    void registerAgent(Agent::SharedPtr agent);

    uint64_t statNotification(const std::string& notificationName) const;

  private:
    void run();

    mutable std::mutex mMutex;                  //< Protect data containers against data race

    std::unique_ptr<std::thread> mThreadPtr;    //< Do internal calculations. Send notifications

    std::condition_variable mConditionVariable; //< For the control of the thread

    Status mStatus;                             //< The status of mThreadPtr
    
    std::vector<Agent::WeakPtr> mAgents;        //< List of agents that registered itself. The agent lifetime is not
                                                //< known. Need to lock to test the existence of it

    std::map<std::string, uint64_t> mNotificationStatistics;  //< Nr. of notifications sent of a given type
  };

}

#endif
