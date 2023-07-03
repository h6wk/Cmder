
#include "Agent.hpp"
#include "Logger.hpp"

#include <chrono>
#include <future>
#include <thread>

std::ostream& operator<<(std::ostream& ostr, const Agent::Mode& mode)
{
  switch (mode)
  {
  case Agent::Async:
    ostr << "async";
    break;
  case Agent::Blocking:
    ostr << "blocking";
    break;
  }
  return ostr;
}


Agent::SharedPtr Agent::create(const Server& server, Callback::SharedPtr callback)
{
  //Use a temporary subclass to make a connection between a smart pointer generator function and a class with a private constructor.
  struct MkSharedEnabler : public Agent {
    MkSharedEnabler(const Server& server, Callback::SharedPtr cb) : Agent(server, cb) {}
  };
  auto instance = std::make_shared<MkSharedEnabler>(server, callback);
  return instance;
  //return std::make_shared<Agent>(server);
}

std::string Agent::doTask(Mode mode, const std::string &task) const
{
  std::string result;
  LOG("Task started in " << mode << " mode");
  if (mCallback) {
    mCallback->notify("Task started: " + task);
  }

  std::future<std::string> futureResult = std::async([]() {

    //pthread_setname_np(pthread_self(), "TH_EXECUTOR");

    using namespace std::chrono_literals;

    LOG("Executor thread start");
    std::this_thread::sleep_for(2000ms);
    LOG("Executor thread stop");
    return std::string("3.14");
    });

  result = futureResult.get();
  LOG("Task ended. Return value: " << result);
  return result;
}

Agent::Agent(const Server& server, Callback::SharedPtr callback)
: mCallback(callback)
{
  LOG("Created");
}

Agent::~Agent()
{
  LOG("Deleted");
}
