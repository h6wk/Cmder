/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-03 12:00:00                               *
 * @LastEditDate          : 2023-07-18 11:03:16                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include <callback/Callback.hpp>
#include <Logger.hpp>
#include <Types.hpp>

#include <iostream>
#include "Callback.hpp"

namespace cmder::cb {

  Callback::Callback()
  : mMutex()
  , mConditionVariable()
  , mOwnerName("N/A")
  , mMessages()
  {
  }

  Callback::~Callback()
  {
    std::lock_guard guard(mMutex);
    LOG("");
    mMessages = {};
  }

  void Callback::notify(TaskId_t taskId, CallbackMessageType_t type, const std::string& message)
  {
    std::lock_guard guard(mMutex);

    mMessages.push(Message_t(std::chrono::system_clock::now(), taskId, type, message));
    LOG("Add: '" << message << "' count: " << mMessages.size() << " " << mOwnerName);

    mConditionVariable.notify_one();
  }

  void Callback::clear()
  {
    std::lock_guard guard(mMutex);
    LOG("");
//    mMessages.clear();
  }

  std::optional<Callback::Message_t> Callback::tryPop()
  {
    std::lock_guard guard(mMutex);

    if (mMessages.empty()) {
      return std::optional<Callback::Message_t>();
    }

    auto retVal= std::make_optional(mMessages.front());
    mMessages.pop();
    return retVal;
  }

  std::optional<Callback::Message_t> Callback::pop()
  {
    std::unique_lock ul(mMutex);

    if (mMessages.empty()) {
      mConditionVariable.wait(ul);
    }

    auto retVal= std::make_optional(mMessages.front());
    mMessages.pop();
    return retVal;
  }

  size_t Callback::size() const
  {
    std::lock_guard guard(mMutex);
    LOG("Callback message size is " << mMessages.size() << " (" << mOwnerName << ")");
    return mMessages.size();
  }

  bool Callback::empty() const
  {
    std::lock_guard guard(mMutex);
    return mMessages.empty();
  }

  void Callback::setOwner(const std::string &owner)
  {
    std::lock_guard guard(mMutex);
    mOwnerName = owner;
  }

  std::ostream& operator<<(std::ostream& ostr, const Callback::Message_t& msg)
  {
    ostr << msg.mTime << ", " << msg.mText;
    return ostr;
  }

  std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb)
  {
    if ( ! cb) {
      ostr << "-";
      return ostr;
    }

    std::lock_guard guard(cb->mMutex);
    //for (auto message : cb->mMessages) {
      //ostr << "\n        (" << message.first.first << "," << message.first.second << "): " << message.second;
    //}
    return ostr;
  }
}
