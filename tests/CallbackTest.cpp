/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-16 00:03:52                               *
 * @LastEditDate          : 2023-07-24 23:20:22                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/

#include "CallbackTest.hpp"

#include <tools/ThreadPool.hpp>

namespace cmder::tst {
  Callback::SharedPtr CallbackTest::sCallback;

  TEST_F(CallbackTest, EmptyCallback)
  {
    ASSERT_TRUE(sCallback);
    EXPECT_EQ(0, sCallback->size());
    EXPECT_TRUE(sCallback->empty());

    auto message = sCallback->tryPop();
    EXPECT_FALSE(message.has_value());
  }

  TEST_F(CallbackTest, TwoThreadPushMessage)
  {
    ASSERT_TRUE(sCallback);

    using cmder::tools::ThreadPool;

    ThreadPool tp(2);

    srand(time(nullptr));
    constexpr size_t NUM_MESSAGES = 1000;
    std::vector<std::future<std::string>> resultFutures;
    resultFutures.reserve(NUM_MESSAGES);

    for (size_t i = 0;i < NUM_MESSAGES; ++i) {

      auto fut = tp.execute(
        []() -> std::string{
          sCallback->notify(NO_TASK_ID, CallbackMessageType_t::NOTIFICATION, "PING");
          // wait 0 .. 9 nanoseconds
          std::this_thread::sleep_for(std::chrono::nanoseconds(rand() % 10));
          return "DONE";
        }
      );

      resultFutures.push_back(std::move(fut));
    }

    for (auto& x : resultFutures) {
      auto res = x.get();
      EXPECT_EQ(res, std::string("DONE"));
    }

    EXPECT_EQ(resultFutures.size(), NUM_MESSAGES);
    EXPECT_EQ(sCallback->size(), NUM_MESSAGES);
  }


} // namespace cmder::tst 
