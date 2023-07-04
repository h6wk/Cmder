#include "Callback.hpp"
#include "Logger.hpp"

#include <chrono>
#include <iostream>

void Callback::notify(const std::string &message)
{
  mMessages.emplace(std::chrono::system_clock::now(), message);
  LOG("Add: '" << message << "' count: " << mMessages.size());
}

std::ostream &operator<<(std::ostream &ostr, const Callback::SharedPtr &cb)
{
  if ( ! cb) {
    ostr << "-";
    return ostr;
  }

  for (auto message : cb->mMessages) {
    ostr << "\n        " << message.first << ": " << message.second;
  }
  return ostr;
}
