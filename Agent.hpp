
#include <memory>
#include <string>

#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

class Agent
{
public:
  using SharedPtr = std::shared_ptr<Agent>;
  enum Mode {Async, Blocking};

  /// @brief Create a new Agent object.
  /// @return Shared pointer on the newly created object.
  static SharedPtr create();

  virtual ~Agent();

  /// @brief Invoke a task and get the result of it
  /// @param mode Blocking the caller or asynch (NOTE: only blocking works now)
  /// @param task Name of task
  /// @return Get the return value
  std::string doTask(Mode mode, const std::string& task) const;

protected:
  Agent();
};

std::ostream& operator<<(std::ostream& ostr, const Agent::Mode& mode);

#endif // AGENT_H_INCLUDED
