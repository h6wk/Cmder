/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-11 23:43:21                              *
 * @LastEditDate          : 2023-07-12 10:52:24                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef C77CE49B_37BF_4C82_9A2E_FCE9676B18A2
#define C77CE49B_37BF_4C82_9A2E_FCE9676B18A2

#include <iosfwd>
#include <iostream>
#include <vector>

namespace cmder::agent {

  enum class TaskName {Pi, BlockMe_3s, PingMe_5x};

  class Task {
  public:
    struct TaskConfig {

    };


  private:
    std::vector<TaskConfig> mTaskConfiguration;

  };

  std::ostream& operator<<(std::ostream& ostr, const TaskName& task);

}

#endif /* C77CE49B_37BF_4C82_9A2E_FCE9676B18A2 */
