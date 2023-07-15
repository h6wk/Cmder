/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-16 00:03:05                              *
 * @LastEditDate          : 2023-07-16 00:06:23                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef FADCA300_E0AC_4E96_908A_23F47B84398D
#define FADCA300_E0AC_4E96_908A_23F47B84398D

#include <gtest/gtest.h>

#include "../Callback.hpp"


namespace cmder::tst {

  class CallbackTest : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
        
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite()
    {
      sCallback = std::make_shared<Callback>();
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
      sCallback.reset();
    }

    // virtual void SetUp() will be called before each test is run.
    void SetUp() override
    {
      sCallback->clear();
    }

    // virtual void TearDown() will be called after each test is run. You should define it if there is cleanup work to do.
    //virtual void TearDown() {
    //}

    static Callback::SharedPtr sCallback;
  };
}



#endif /* FADCA300_E0AC_4E96_908A_23F47B84398D */
