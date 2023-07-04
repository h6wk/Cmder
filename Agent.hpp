#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Callback.hpp"
#include "Receipt.hpp"
#include "Server.hpp"

#include <memory>
#include <string>


/**
 * Client uses the agent to get access to a service. This represents a layer
 * between the client and the service layer. In a multi service environment the
 * agent can select the proper service provider.
 * 
 * Accepts the task and forwards to the selected service. Accepts the service's
 * response and distributes to the client's callback.
 */
class Agent
{
public:
  using SharedPtr = std::shared_ptr<Agent>;
  enum Task {Pi, BlockMe_3s, PingMe_5x};
  enum ReturnCode {OK, FAIL};


  /// @brief Create a new Agent object.
  /// @param server Reference to the server that will be called to do the task
  /// @param callback Shared pointer to the callback (async!)
  ///                 Empty pointer -> only blocking mode is possible
  /// @return Shared pointer on the newly created object.
  static SharedPtr create(const Server& server, Callback::SharedPtr callback);

  virtual ~Agent();

  /// @brief Invoke a task and get the result of it
  /// @param mode Blocking the caller or asynch (NOTE: only blocking works now)
  /// @param task Name of task
  /// @param result The result in blocking mode. In async mode the result is pushed
  ///         into the callbak.
  /// @return Get back a receipt of ackowledge that can be used to find the corresponding
  ///         async result.
  Receipt doTask(Receipt::Mode mode, Task task, std::string& result) const;

private:
  explicit Agent(const Server& server, Callback::SharedPtr callback);

  Callback::SharedPtr mCallback;  //< Client's callback to send async responses
  const Server& mServer;          //< As of now the only service. Later the
                                  //< service will be selected for a task based on
                                  //< some selection criteria.
};

std::ostream& operator<<(std::ostream& ostr, const Agent::Task& task);

#endif // AGENT_H_INCLUDED
