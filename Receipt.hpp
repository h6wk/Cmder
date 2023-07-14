/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-05 12:00:00                              *
 * @LastEditDate          : 2023-07-13 22:26:48                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef F39AA2B7_290B_47E7_A935_50D0339ECDA3
#define F39AA2B7_290B_47E7_A935_50D0339ECDA3

#include <chrono>
#include <cstdint>

using TaskId = uint32_t;

#define INVALID_TASK_ID 0u

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

#endif /* F39AA2B7_290B_47E7_A935_50D0339ECDA3 */
