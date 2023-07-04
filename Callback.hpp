#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

#include <chrono>
#include <map>
#include <memory>
#include <string>

class Callback
{
public:
  using SharedPtr = std::shared_ptr<Callback>;
  
  Callback() = default;
  virtual ~Callback() = default;

  void notify(const std::string& message);

  friend std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

private:
  std::map<std::chrono::time_point<std::chrono::system_clock>, std::string> mMessages;
};

std::ostream& operator<<(std::ostream& ostr, const Callback::SharedPtr& cb);

#endif