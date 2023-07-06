
#include "Agent.hpp"
#include "Logger.hpp"

#include <chrono>
#include <future>
#include <thread>


std::ostream& operator<<(std::ostream& ostr, const Agent::Task& task)
{
  switch (task) {
  case Agent::Task::BlockMe_3s:
    ostr << "BLOCK_ME_3s";
    break;
  case Agent::Task::Pi:
    ostr << "PI";
    break;
  case Agent::Task::PingMe_5x:
    ostr << "PING_ME_5x";
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
}


Receipt Agent::doTask(Receipt::Mode mode, Agent::Task task, std::string& result) const
{
  Receipt receipt(mode);

  result.clear();
  LOG("Task '" << task << "' started in " << mode << " mode");
  if (mCallback) {
    mCallback->notify(receipt.getTaskId(), Callback::NOTIFICATION, "Task started: " + task);
  }

  std::future<std::string> futureResult = std::async([]() {

    //pthread_setname_np(pthread_self(), "TH_EXECUTOR");

    using namespace std::chrono_literals;

    LOG("Executor thread start");
    std::this_thread::sleep_for(2000ms);
    LOG("Executor thread stop");
    return std::string("3.14");
    });


  switch (mode) {
  case Receipt::Async:
    // TODO: register the future in a store, put the result later into the callback
    break;
  case Receipt::Blocking:
    result = futureResult.get();
    break;
  }

  LOG("Leaving method with return value: " << result);

  receipt.setEndTime();
  return receipt;
}

Agent::Agent(const Server& server, Callback::SharedPtr callback)
: mCallback(callback)
, mServer(server)
{
  LOG("Created");
}

Agent::~Agent()
{
  LOG("Deleted");
}
