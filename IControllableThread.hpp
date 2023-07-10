/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-09 23:06:37                              *
 * @LastEditDate          : 2023-07-10 13:10:53                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef ICONTROLLABLETHREAD_H_INCLUDED
#define ICONTROLLABLETHREAD_H_INCLUDED

namespace Cmder {

  enum class Status
  {
    Init, Start, Run, Stop
  };

  class IControllableThread
  {
  public:
    /// @brief Start the internal working thread.
    virtual void start() = 0;

    /// @brief Ask the internal thread to leave and stop. No more nofitications
    ///        or no new tasks are going to be accepted.
    virtual void stop() = 0;

    /// @brief Get back the status value
    /// @return Enum value
    virtual Status getStatus() const = 0;
  };

}

#endif
