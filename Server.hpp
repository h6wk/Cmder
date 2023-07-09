#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Agent.hpp"

#include <condition_variable>
#include <memory>
#include <thread>

class Server {
public:
  enum Status {Init, Start, Run, Stop};
  
  Server();
  virtual ~Server();

  /// @brief Start the internal working thread.
  void start();

  /// @brief Ask the internal thread to leave and stop. No more nofitications
  ///        or no new tasks are going to be accepted.
  void stop();

  /// @brief Get back the status value
  /// @return Enum value
  Status getStatus() const;

  /// @brief Registers an agent into the server (for notifications)
  /// @param agent Shared pointer to the agent
  void registerAgent(Agent::SharedPtr agent);

private:
  void run();

  std::unique_ptr<std::thread> mThreadPtr;

  mutable std::mutex mMutex;
  std::condition_variable mConditionVariable;

  Status mStatus;

  std::vector<Agent::WeakPtr> mAgents;        //< List of agents that registered itself. The agent lifetime is not
                                              //< known. Need to lock to test the existence of it.
};

#endif
