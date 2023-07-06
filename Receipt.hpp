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
  enum Status {OK, FAIL};
  using ChronotTime_t = std::chrono::time_point<std::chrono::system_clock>;

  /// @brief Construct a receipt up to the mode.
  /// @param mode 
  explicit Receipt(Mode mode);

  /// @brief Set the end time just before the return to the client
  void setEndTime();

  /// @brief Get the execution mode
  /// @return mode value 
  Mode getExecutionMode() const;

  /// @brief Get the task id. Unique id representing the task.
  /// @return Integer value representing the task
  TaskId getTaskId() const;

  const ChronotTime_t& getStartTime() const;
  const ChronotTime_t& getEndTime() const;

  Status getStatus() const;
  void setStatus(Status status);
private:
  const Mode mMode;
  const TaskId mTaskId;
  ChronotTime_t mStartTime;
  ChronotTime_t mEndTime;
  Status mStatus;
};

std::ostream& operator<<(std::ostream& ostr, const Receipt::Mode& mode);

#endif