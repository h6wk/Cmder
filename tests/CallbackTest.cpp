/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-16 00:03:52                              *
 * @LastEditDate          : 2023-07-18 10:31:49                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "CallbackTest.hpp"


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

    

  }


} // namespace cmder::tst 
