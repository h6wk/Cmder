#include "Callback.hpp"
#include <iostream>

void Callback::notify(const std::string &message)
{
  mMessages.emplace(std::chrono::system_clock::now(), message);
}

std::ostream &operator<<(std::ostream &ostr, const Callback &cb)
{
  for (auto message : cb.mMessages) {
    std::cout << "\n" << message.first << ": " << message.second;
  }
  return ostr;
}
