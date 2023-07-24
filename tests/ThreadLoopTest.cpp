/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-22 22:50:35                              *
 * @LastEditDate          : 2023-07-24 09:12:38                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "ThreadLoopTest.hpp"

namespace cmder::tst
{
  TEST_F(ThreadPoolTest, LongerAndLongerTasks)
  {
    ThreadPool tp(3);

    std::vector<std::future<size_t>> results;

    for (size_t i = 0;i < 8; ++i) {
      auto fut = tp.execute(
        [i]() {
          LOG("Task wait time:" << i << " second(s)");
          std::this_thread::sleep_for(std::chrono::seconds(i));
          return i;
        }
      );
      results.emplace_back(std::move(fut));
    }

    for (auto &result : results) {
      std::cout << result.get() << ' ';
    }
  }
    
} // namespace cmder::tst
