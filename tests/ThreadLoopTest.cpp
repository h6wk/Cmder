/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-22 22:50:35                              *
 * @LastEditDate          : 2023-07-24 16:07:11                              *
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

  bool isAscending(std::vector<int> &nums, int l, int r)
  {
      for (int i = l; i + 1 < r; ++i)
          if (nums[i] > nums[i + 1])
              return false;
      return true;
  }

  TEST_F(ThreadPoolTest, TaskWithParam)
  {
    constexpr int N = 1e7;
    std::vector<int> nums(N);

    srand(time(nullptr));
    for (int i = 0;i < N; ++i) {
      nums[i] = rand();
    }

    ThreadPool tp(4);
    std::vector<std::future<std::pair<int, int>>> res;
    constexpr int step = N/4;

    for (int i = 0;i < 4; ++i) {
      auto future = tp.execute(
        [&nums](int l, int r) {
          std::sort(nums.begin() + l, nums.begin() + r);
          return std::make_pair(l, r);
        }
        , i * step
        , (i + 1) * step
      );

      res.emplace_back(std::move(future));
    }

    for (auto& x : res) {
        auto [l, r] = x.get();
        EXPECT_TRUE(isAscending(nums, l, r));
        std::printf("Pass [%d, %d). \n", l, r); 
    }

  }
    
} // namespace cmder::tst
