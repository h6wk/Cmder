/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-05 12:00:00                              *
 * @LastEditDate          : 2023-07-13 22:27:13                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

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
, mEndTime()
, mStatus(OK)
{
}

void Receipt::setEndTime()
{
  mEndTime = std::chrono::system_clock::now();
}

Receipt::Mode Receipt::getExecutionMode() const
{
  return mMode;
}

TaskId Receipt::getTaskId() const
{
  return mTaskId;
}

const Receipt::ChronotTime_t& Receipt::getStartTime() const
{
  return mStartTime;
}

const Receipt::ChronotTime_t& Receipt::getEndTime() const
{
    return mEndTime;
}

Receipt::Status Receipt::getStatus() const
{
  return mStatus;
}

void Receipt::setStatus(Status status)
{
  mStatus = status;
}
