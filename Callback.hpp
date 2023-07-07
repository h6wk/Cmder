#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

#include "Receipt.hpp"

#include <chrono>
#include <map>
#include <memory>
#include <string>


/**
 * Thread safe container to store the notifications / results between the Agent and the User
 */
class Callback
{
public:
  using SharedPtr = std::shared_ptr<Callback>;
  
  enum Type {NOTIFICATION, RESULT};
  
  struct Message_t {
    Receipt::ChronotTime_t mTime;
    std::string mResult;
  };

  Callback() = default;
  virtual ~Callback();

  /// @brief Add a new message into the callback.
  void notify(TaskId taskId, Type type, const std::string& message);

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

  friend std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

private:

  using Key_t = std::pair<TaskId, Type>;

  std::map<Key_t, Message_t> mMessages;
  std::mutex mMutex;
};

std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

#endif