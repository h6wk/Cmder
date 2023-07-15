/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-03 12:00:00                               *
 * @LastEditDate          : 2023-07-15 21:47:14                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include "Callback.hpp"
#include "Logger.hpp"
#include <Types.hpp>

#include <iostream>

namespace cmder {

  Callback::Callback()
  : mMutex()
  , mOwnerName("N/A")
  , mMessages()
  {
  }

  Callback::~Callback()
  {
    std::lock_guard guard(mMutex);
    LOG("");
    mMessages.clear();
  }

  void Callback::notify(TaskId_t taskId, Callback::Type type, const std::string& message)
  {
    std::lock_guard guard(mMutex);

//    mMessages.emplace(
//      std::make_pair(taskId, type)
//      , Message_t(std::chrono::system_clock::now(), message));
    mMessages.push_back(Message_t(std::chrono::system_clock::now(), taskId, type, message));
    LOG("Add: '" << message << "' count: " << mMessages.size() << " " << mOwnerName);
  }

  void Callback::clear()
  {
    std::lock_guard guard(mMutex);
    LOG("");
    mMessages.clear();
  }

  std::optional<Callback::Message_t> Callback::getFirst(const Receipt& receipt, Callback::Type type)
  {
    std::lock_guard guard(mMutex);

    auto it = std::find_if(mMessages.begin(), mMessages.end()
      , [&receipt, &type](const Message_t& mess){
          return mess.mTaskId == receipt.getTaskId() && mess.mType == type;
      });
    if (it != mMessages.end()) {
      return std::make_optional(*it);
    }
//    auto it = mMessages.find_if(std::make_pair(receipt.getTaskId(), type));
//    if (it != mMessages.end()) {
//      return std::make_optional(it->second);
//    }

    return std::optional<Callback::Message_t>();
  }

  std::optional<Callback::Message_t> Callback::waitFirst(const Receipt& receipt, Type type)
  {
//    const Key_t key(std::make_pair(receipt.getTaskId(), type));

    // Node handle is a move-only type that owns and provides access to the element => will be removed from the container!!!
    while (true) {

      std::lock_guard guard(mMutex);

//      auto nodeHandle = mMessages.extract(key);
//      if (nodeHandle) {
//        return std::make_optional<Message_t>(
//          nodeHandle.mapped().mTime, nodeHandle.mapped().mResult);
//      }

      auto it = std::find_if(mMessages.begin(), mMessages.end()
        , [&receipt, &type](const Message_t& mess){
            return mess.mTaskId == receipt.getTaskId() && mess.mType == type;
        });
      if (it != mMessages.end()) {
        return std::make_optional(*it);
      }


      // TODO: sleep + check time limit! -> break;
    }

    return std::optional<Message_t>();
  }

  size_t Callback::messagesSize() const
  {
    std::lock_guard guard(mMutex);
    LOG("Callback message size is " << mMessages.size() << " (" << mOwnerName << ")");
    return mMessages.size();
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
