#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

#include "Receipt.hpp"

#include <chrono>
#include <map>
#include <memory>
#include <string>

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
  virtual ~Callback() = default;

  void notify(TaskId taskId, Type type, const std::string& message);

  void clear();

  /// @brief Get back the corresponding result if exists
  /// @param receipt Receipt that identifies the response
  /// @return An std::optional that might include the Message (or not).
  std::optional<Message_t> getFirst(const Receipt& receipt, Type type);

  friend std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);


private:
  using Key_t = std::pair<TaskId, Type>;
  std::map<Key_t, Message_t> mMessages;
};

std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

#endif