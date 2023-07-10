/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-01 12:00:00                               *
 * @LastEditDate          : 2023-07-10 23:42:51                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include "Agent.hpp"
#include "Logger.hpp"
#include "Server.hpp"

#include <assert.h>
#include <chrono>
#include <future>
#include <random>
#include <thread>


namespace Cmder {

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


  std::ostream& operator<<(std::ostream& ostr, const Status& status)
  {
    switch (status) {
    case Status::Init:
      ostr << "INIT";
      break;
    case Status::Run:
      ostr << "RUN";
      break;
    case Status::Start:
      ostr << "START";
      break;
    case Status::Stop:
      ostr << "STOP";
      break;
    }
    return ostr;
  }


  Agent::SharedPtr Agent::create(Server& server, Callback::SharedPtr callback)
  {
    static const std::array<std::string, 6> sAgentNames{
      "Mulder", "Scully", "Bond", "Bourne", "Hunt", "Nikita"
    };
    
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distrName(0, sAgentNames.max_size());
    static std::uniform_int_distribution<int> distrId(100, 999);
    std::stringstream ss;
    ss << "<<" << sAgentNames[distrName(generator)] << "_" << distrId(generator) << ">>";

    //Use a temporary subclass to make a connection between a smart pointer generator function and a class with a private constructor.
    struct MkSharedEnabler : public Agent {
      MkSharedEnabler(const Server& server, Callback::SharedPtr cb, const std::string& name) : Agent(server, cb, name) {}
    };
    auto instance = std::make_shared<MkSharedEnabler>(server, callback, ss.str());
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
    LOG("Agent " << mDebugName << " got message: " << message);

    std::lock_guard guard(mMutex);
    mNotifications.push(message);
  }

  const std::string &Agent::getName() const
  {
    return mDebugName;
  }

  uint64_t Agent::statNotification(const std::string &notificationName) const
  {
    std::lock_guard guard(mMutex);
    return mNotifications.size();
  }

  Agent::Agent(const Server &server, Callback::SharedPtr callback, const std::string& name)
  : mMutex()
  , mThreadPtr()
  , mConditionVariable()
  , mServer(server)
  , mCallback(callback)
  , mNotifications()
  , mDebugName(name)
  {
    {
      std::lock_guard guard(mMutex);
      mStatus = Status::Init;
    }

    LOG("Agent " << mDebugName << " created. Status set to " << mStatus);
  }

  void Agent::run()
  {
    using namespace std::chrono_literals;

    {
      std::lock_guard guard(mMutex);
      mStatus = Status::Run;
    }

    LOG("Agent runs");

    while (true) {

      std::unique_lock ul(mMutex);
      if (mStatus == Status::Stop) {break;}

      mConditionVariable.wait_for(ul, 500ms);
    }

    LOG("Agent-thread quits");
  }

  Agent::~Agent()
  {
    stop();

    if (mThreadPtr) {
      mThreadPtr->join();
    }
    LOG("Deleted");
  }

  void Agent::start()
  {
    // restart is not supported! need to destroy the object and join the thread 
    assert(mStatus == Status::Init);

    mThreadPtr = std::make_unique<std::thread>(&Agent::run, this);
    std::lock_guard guard(mMutex);
    mStatus = Status::Start;
  }

  void Agent::stop()
  {
    {
      std::lock_guard guard(mMutex);
      mStatus = Status::Stop;
    }

    // Don't need to hold the lock while calling notify_all()
    mConditionVariable.notify_all();
  }

  Status Agent::getStatus() const
  {
    std::lock_guard guard(mMutex);
    return mStatus;
  }
}
