/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-05 12:00:00                               *
 * @LastEditDate          : 2023-07-18 11:38:35                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include <agent/Receipt.hpp>

#include <Types.hpp>

#include <atomic>
#include <iostream>


namespace cmder::agent {

  using namespace cmder;

  static std::atomic<TaskId_t> sNextTaskId = 0; 

  std::ostream& operator<<(std::ostream& ostr, const ExecutionMode_t& mode)
  {
    switch (mode) {
    case ExecutionMode_t::Async:
      ostr << "async";
      break;
    case ExecutionMode_t::Blocking:
      ostr << "blocking";
      break;
    }
    return ostr;
  }

  Receipt::Receipt(ExecutionMode_t mode)
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

  ExecutionMode_t Receipt::getExecutionMode() const
  {
    return mMode;
  }

  TaskId_t Receipt::getTaskId() const
  {
    return mTaskId;
  }

  const ChronoTime_t& Receipt::getStartTime() const
  {
    return mStartTime;
  }

  const ChronoTime_t& Receipt::getEndTime() const
  {
    return mEndTime;
  }

  Receipt::Status_t Receipt::getStatus() const
  {
    return mStatus;
  }

  void Receipt::setStatus(Status_t status)
  {
    mStatus = status;
  }

}