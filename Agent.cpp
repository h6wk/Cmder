/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-01 12:00:00                               *
 * @LastEditDate          : 2023-07-09 22:59:50                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include "Agent.hpp"
#include "Logger.hpp"
#include "Server.hpp"

#include <assert.h>
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


Agent::SharedPtr Agent::create(Server& server, Callback::SharedPtr callback)
{
  //Use a temporary subclass to make a connection between a smart pointer generator function and a class with a private constructor.
  struct MkSharedEnabler : public Agent {
    MkSharedEnabler(const Server& server, Callback::SharedPtr cb) : Agent(server, cb) {}
  };
  auto instance = std::make_shared<MkSharedEnabler>(server, callback);
  server.registerAgent(instance);
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
  else {
    assert(mode != Receipt::Async);
  }

  // This is the Agent's internal thread!
  std::future<std::string> futureResult = std::async([&mode, this, &receipt]() {

    //pthread_setname_np(pthread_self(), "TH_EXECUTOR");

    using namespace std::chrono_literals;

    LOG("Executor thread start");
    std::this_thread::sleep_for(1000ms);
    LOG("Executor thread stop");

    const std::string result("3.14");

    if (mode == Receipt::Async) {
      this->mCallback->notify(receipt.getTaskId(), Callback::RESULT, result);
      return std::string("");
    }

    return result;
    });


  switch (mode) {
  case Receipt::Async:
    // TODO: register the future in a store, put the result later into the callback
    break;
  case Receipt::Blocking:
    // The client's thred is blocked until the future is not calculated!
    result = futureResult.get();
    break;
  }

  LOG("Leaving method with return value: " << result);

  receipt.setEndTime();
  return receipt;
}

void Agent::notify(const std::string &message)
{
  LOG("Message: " << message);
}

Agent::Agent(const Server &server, Callback::SharedPtr callback)
: mCallback(callback)
, mServer(server)
, mThreadPtr()
, mMutex()
{
  LOG("Created");
}

Agent::~Agent()
{
  LOG("Deleted");
}
