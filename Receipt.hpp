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
  enum Mode {Async, Blocking};

  explicit Receipt(Mode mode);

  void setEndTime();

private:
  const Mode mMode;
  const TaskId mTaskId;
  std::chrono::time_point<std::chrono::system_clock> mStartTime;
  std::chrono::time_point<std::chrono::system_clock> mEndTime;
  // acceptance;
};

std::ostream& operator<<(std::ostream& ostr, const Receipt::Mode& mode);

#endif