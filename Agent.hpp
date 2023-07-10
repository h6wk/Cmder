/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-01 12:00:00                              *
 * @LastEditDate          : 2023-07-10 15:03:17                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Callback.hpp"
#include "IControllableThread.hpp"
#include "Receipt.hpp"

#include <condition_variable>
#include <memory>
#include <string>
#include <thread>

// -- forward declaration
class Server;

namespace Cmder {

  /**
   * Client uses the agent to get access to a service. This represents a layer
   * between the client and the service layer. In a multi service environment the
   * agent can select the proper service provider.
   * 
   * Accepts the task and forwards to the selected service. Accepts the service's
   * response and distributes to the client's callback.
   */
  class Agent : public IControllableThread
  {
  public:
    using SharedPtr = std::shared_ptr<Agent>;
    using WeakPtr = std::weak_ptr<Agent>;

    enum Task {Pi, BlockMe_3s, PingMe_5x};

    /// @brief Create a new Agent object.
    /// @param server Reference to the server that will be called to do the task
    /// @param callback Shared pointer to the callback (async!)
    ///                 Empty pointer -> only blocking mode is possible
    /// @return Shared pointer on the newly created object.
    static SharedPtr create(Server& server, Callback::SharedPtr callback);

    virtual ~Agent();

    // IControllableThread interfaces:
    void start() override;
    void stop() override;
    Status getStatus() const override;

    /// @brief Invoke a task and get the result of it
    /// @param mode Blocking the caller or asynch (NOTE: only blocking works now)
    /// @param task Name of task
    /// @param result The result in blocking mode. In async mode the result is pushed
    ///         into the callbak.
    /// @return Get back a receipt of ackowledge that can be used to find the corresponding
    ///         async result.
    Receipt doTask(Receipt::Mode mode, Task task, std::string& result) const;

    /// @brief Server uses this interface to send notifications to the agent
    /// @param message Notification
    void notify(const std::string& message);

  private:
    explicit Agent(const Server& server, Callback::SharedPtr callback);

    /// @brief The method that will be executed by the working thread.
    void run();

    Callback::SharedPtr mCallback;            //< Client's callback to send async responses

    const Server& mServer;                    //< As of now the only service. Later the
                                              //< service will be selected for a task based on
                                              //< some selection criteria.

    std::unique_ptr<std::thread> mThreadPtr;  //< Thread responsible to accept notifications from
                                              //< the server and forward them to the client.
                                              //< NOTE: another worker thread is created to execute the task (doTask()).

    mutable std::mutex mMutex;                //< Protect data containers against data race.

    Status mStatus;                           //< The status of mThreadPtr.

    std::condition_variable mConditionVariable; //< For the control of the thread

    std::string mDebugName;                   //< To name (ID) for better logging
  };

  std::ostream& operator<<(std::ostream& ostr, const Agent::Task& task);
  std::ostream& operator<<(std::ostream& ostr, const Status& status);

}

#endif // AGENT_H_INCLUDED
