#include "Callback.hpp"
#include "Logger.hpp"

#include <chrono>
#include <iostream>

void Callback::notify(TaskId taskId, Callback::Type type, const std::string& message)
{
  mMessages.emplace(
    std::make_pair(taskId, type)
    , Message_t(std::chrono::system_clock::now(), message));
  LOG("Add: '" << message << "' count: " << mMessages.size());
}

void Callback::clear()
{
  mMessages.clear();
}

std::optional<Callback::Message_t> Callback::getFirst(const Receipt& receipt, Callback::Type type)
{
  auto it = mMessages.find(std::make_pair(receipt.getTaskId(), type));
  if (it != mMessages.end()) {
    return std::make_optional(it->second);
  }

  return std::optional<Callback::Message_t>();
}

std::ostream& operator<<(std::ostream &ostr, const Callback::Message_t& msg)
{
  ostr << msg.mTime << ", " << msg.mResult;
  return ostr;
}

std::ostream& operator<<(std::ostream &ostr, const Callback::SharedPtr& cb)
{
  if ( ! cb) {
    ostr << "-";
    return ostr;
  }

  for (auto message : cb->mMessages) {
    ostr << "\n        (" << message.first.first << "," << message.first.second << "): " << message.second;
  }
  return ostr;
}
