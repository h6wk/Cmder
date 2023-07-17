/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-03 12:00:00                               *
 * @LastEditDate          : 2023-07-17 00:14:54                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#ifndef BC631564_DD35_4804_91C9_7CE0EAC96FFF
#define BC631564_DD35_4804_91C9_7CE0EAC96FFF

#include "../../Receipt.hpp"

#include <Types.hpp>

#include <condition_variable>
#include <queue>
#include <memory>
#include <string>

namespace cmder::cb {

  /**
   * Object shared between the entities communicating with eachother asynchronously.
   * 
   * The owner of the object is supposed to be the "receiver", that expects to receive
   * the message from the "sender" some time later. The "receiver" keeps a shared pointer
   * on the callback, the "sender" a weak pointer.
   * 
   * If the "receiver" cannot lock the weak pointer anymore, the "receiver" lifetime has
   * been ended.
   */
  class Callback
  {
  public:
    using SharedPtr = std::shared_ptr<Callback>;
    using WeakPtr = std::weak_ptr<Callback>;    

    struct Message_t {
      ChronoTime_t mTime;
      TaskId_t mTaskId;
      CallbackMessageType_t mType;
      std::string mText;
    };

    Callback();
    virtual ~Callback();

    /// @brief Add a new message into the callback's queue
    void notify(TaskId_t taskId, CallbackMessageType_t type, const std::string& message);

    /// @brief Clean the messages from the callback's queue
    void clear();

    /// @brief Get back a message (if exists)
    ///        Note, this is a non-blocking call!
    /// @return An std::optional that might include the Message (or not).
    std::optional<Message_t> tryPop();

    /// @brief Wait until a type of message arrives into the callback. This is a BLOCKING call!
    ///        Note, if the message exists, will be moved from the callback into the std::optional
    /// @return An std::optional that might include the Message (or not).
    std::optional<Message_t> pop();

    size_t messagesSize() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

    void setOwner(const std::string& owner);
  private:

    mutable std::mutex mMutex;                  //< Protect data containers against data race
    std::condition_variable mConditionVariable; //< For the sync between threads


    std::string mOwnerName;

    std::queue<Message_t> mMessages;            //< Message queue
  };

  std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

}

#endif /* BC631564_DD35_4804_91C9_7CE0EAC96FFF */
