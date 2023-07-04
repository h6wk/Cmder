#ifndef RECEIPT_H_INCLUDED
#define RECEIPT_H_INCLUDED

#include <chrono>
#include <cstdint>

using TaskId = uint32_t;

/**
 * Ackowledgement of recipt (of the task)
 */
class Receipt
{
public:
  explicit Receipt();

private:
  const TaskId mTaskId;
  std::chrono::time_point<std::chrono::system_clock> mStartTime;
  // acceptance;
};

#endif