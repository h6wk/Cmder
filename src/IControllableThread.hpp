/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-09 23:06:37                              *
 * @LastEditDate          : 2023-07-13 15:20:16                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef C23E3281_F7B0_4080_B75D_90A6A3DB4848
#define C23E3281_F7B0_4080_B75D_90A6A3DB4848

namespace cmder {

  enum class Status
  {
    Init, Start, Run, Stop
  };

  class IControllableThread {
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

#endif /* C23E3281_F7B0_4080_B75D_90A6A3DB4848 */
