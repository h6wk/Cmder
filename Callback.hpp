/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-03 12:00:00                               *
 * @LastEditDate          : 2023-07-15 21:56:29                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#ifndef BC631564_DD35_4804_91C9_7CE0EAC96FFF
#define BC631564_DD35_4804_91C9_7CE0EAC96FFF

#include "Receipt.hpp"

#include <Types.hpp>

#include <map>
#include <memory>
#include <string>

namespace cmder {

  /**
   * Thread safe container to store the notifications / results between the Agent and the User
   */
  class Callback
  {
  public:
    using SharedPtr = std::shared_ptr<Callback>;
    using WeakPtr = std::weak_ptr<Callback>;    
    
    enum Type {NOTIFICATION, RESULT};
    
    struct Message_t {
      ChronoTime_t mTime;
      TaskId_t mTaskId;
      Type mType;
      std::string mText;
    };

    Callback();
    virtual ~Callback();

    /// @brief Add a new message into the callback.
    void notify(TaskId_t taskId, Type type, const std::string& message);

    /// @brief Clean the messages
    void clear();

    /// @brief Get back the corresponding result if it exists.
    ///        Note, if the message exists, will be moved from the callback into the std::optional
    /// @param receipt Receipt that identifies the response
    /// @param type The type of message we are interested in
    /// @return An std::optional that might include the Message (or not).
    std::optional<Message_t> getFirst(const Receipt& receipt, Type type);

    /// @brief Wait until a type of message arrives into the callback. This is a BLOCKING call!
    ///        Note, if the message exists, will be moved from the callback into the std::optional
    /// @param receipt Receipt that identifies the response
    /// @param type The type of message we are interested in
    /// @return An std::optional that might include the Message (or not).
    std::optional<Message_t> waitFirst(const Receipt& receipt, Type type);

    size_t messagesSize() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

    void setOwner(const std::string& owner);
  private:

    mutable std::mutex mMutex;

    std::string mOwnerName;

    std::vector<Message_t> mMessages;
  };

  std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

}

#endif /* BC631564_DD35_4804_91C9_7CE0EAC96FFF */
