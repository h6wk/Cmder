#include "Receipt.hpp"

#include <atomic>
#include <iostream>

static std::atomic<TaskId> sNextTaskId = 0; 


std::ostream& operator<<(std::ostream& ostr, const Receipt::Mode& mode)
{
  switch (mode) {
  case Receipt::Async:
    ostr << "async";
    break;
  case Receipt::Blocking:
    ostr << "blocking";
    break;
  }
  return ostr;
}


Receipt::Receipt(Receipt::Mode mode)
: mMode(mode)
, mTaskId(sNextTaskId++)
, mStartTime(std::chrono::system_clock::now())
{
}

void Receipt::setEndTime()
{
  mEndTime = std::chrono::system_clock::now();
}
