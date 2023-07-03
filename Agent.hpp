#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Callback.hpp"
#include "Server.hpp"

#include <memory>
#include <string>


class Agent
{
public:
  using SharedPtr = std::shared_ptr<Agent>;
  enum Mode {Async, Blocking};

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
  /// @return Get the return value
  std::string doTask(Mode mode, const std::string& task) const;

protected:
  explicit Agent(const Server& server, Callback::SharedPtr callback);
};

std::ostream& operator<<(std::ostream& ostr, const Agent::Mode& mode);

#endif // AGENT_H_INCLUDED
