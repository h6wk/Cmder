/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-01 12:00:00                              *
 * @LastEditDate          : 2023-07-12 23:42:33                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef C5CEADB5_5E42_402E_B2D4_5B6CAA2A11FA
#define C5CEADB5_5E42_402E_B2D4_5B6CAA2A11FA

#include <agent/Task.hpp>

#include "../../Callback.hpp"
#include "../../IControllableThread.hpp"
#include "../../IStatProvider.hpp"
#include "../../Receipt.hpp"

#include <condition_variable>
#include <memory>
#include <queue>
#include <string>
#include <thread>

namespace Cmder {

  // -- forward declaration
  class Server;
}


namespace cmder::agent {

  using namespace Cmder;


  
  /**
   * Client uses the agent to get access to a service. This represents a layer
   * between the client and the service layer. In a multi service environment the
   * agent can select the proper service provider.
   * 
   * Accepts the task and forwards to the selected service. Accepts the service's
   * response and distributes to the client's callback.
   */
  class Agent : public IControllableThread, public IStatProvider
  {
  public:
    using SharedPtr = std::shared_ptr<Agent>;
    using WeakPtr = std::weak_ptr<Agent>;

    /// @brief Create a new Agent object.
    /// @param server Reference to the server that will be called to do the task
    /// @param callback Shared pointer to the callback (async!)
    ///                 Empty pointer -> only blocking mode is possible
    /// @return Shared pointer on the newly created object.
    static SharedPtr create(Cmder::Server& server, Callback::SharedPtr callback);

    virtual ~Agent();

    //////////////////////////////////////////////////////////////////////////
    // IControllableThread abstract interface implementations:              //
    void start() override;                                                  //
    void stop() override;                                                   //
    Status getStatus() const override;                                      //
    //////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////////////
    // IStatProvider abstract interface implementations:                            //
    uint64_t statNotification(const std::string& notificationName) const override;  //
    //////////////////////////////////////////////////////////////////////////////////


    /// @brief Invoke a task and get the result of it
    /// @param mode Blocking the caller or asynch (NOTE: only blocking works now)
    /// @param task Name of task
    /// @param result The result in blocking mode. In async mode the result is pushed
    ///         into the callbak.
    /// @return Get back a receipt of ackowledge that can be used to find the corresponding
    ///         async result.
    Receipt doTask(Receipt::Mode mode, const TaskName& task, std::string& result) const;

    /// @brief Server uses this interface to send notifications to the agent
    /// @param message Notification
    void notify(const std::string& message);

    /// @brief Get the name of the agent (for debugging).
    ///        It is automatically generated at the time of creation.
    /// @return Name as a string constant reference.
    const std::string& getName() const;

    // Helpers to set up/break down the link between the agent and the server (for testing purposes)
    void registerAgent();
    void unregisterAgent();

  private:
    explicit Agent(Server& server, Callback::SharedPtr callback);

    /// @brief The method that will be executed by the working thread.
    void run();

    mutable std::mutex mMutex;                //< Protect data containers against data race

    std::unique_ptr<std::thread> mThreadPtr;  //< Thread responsible to accept notifications from
                                              //< the server and forward them to the client.
                                              //< NOTE: another worker thread is created to execute the task (doTask()).

    std::condition_variable mConditionVariable; //< For the control of the thread

    Status mStatus;                           //< The status of mThreadPtr

    Server& mServer;                          //< As of now the only service. Later the
                                              //< service will be selected for a task based on
                                              //< some selection criteria.

    Callback::WeakPtr mClientCallback;        //< Client's callback to send async responses
                                              //< Since this ia a weak pointer, agent doesn't own it!

    Callback::SharedPtr mCallback;            //< Agent's callback to receive notifications and async responses from the server

    std::queue<std::string> mNotifications;   //< Notification queue (recevied from the server)

    std::string mDebugName;                   //< To name (ID) for better logging
  };

  std::ostream& operator<<(std::ostream& ostr, const Status& status);

}

#endif /* C5CEADB5_5E42_402E_B2D4_5B6CAA2A11FA */
