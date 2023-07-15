/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-15 21:19:21                              *
 * @LastEditDate          : 2023-07-15 22:08:22                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef BAF235F8_DE14_4514_A6AA_2AEBFCBB9836
#define BAF235F8_DE14_4514_A6AA_2AEBFCBB9836

#include <chrono>
#include <cstdint>


#define INVALID_TASK_ID 0u


namespace cmder {

  using ChronoTime_t = std::chrono::time_point<std::chrono::system_clock>;

  using TaskId_t = uint32_t;

  enum class ExecutionMode_t {
    Async
    , Blocking
  };

}


#endif /* BAF235F8_DE14_4514_A6AA_2AEBFCBB9836 */
