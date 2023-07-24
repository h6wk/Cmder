/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-22 22:50:41                              *
 * @LastEditDate          : 2023-07-24 16:06:35                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef B1CC2146_6FBC_4BA6_855C_625AA394500A
#define B1CC2146_6FBC_4BA6_855C_625AA394500A

#include <tools/ThreadPool.hpp>

//  mingw-w64-x86_64-gtest !
#include <gtest/gtest.h>


namespace cmder::tst {

  using namespace cmder;
  using namespace cmder::tools;

  // To use gtest fixture, derive from testing::Test 
  class ThreadPoolTest : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
    
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite() {
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() {
    }

    // virtual void SetUp() will be called before each test is run.
    void SetUp() override {
    }

    // virtual void TearDown() will be called after each test is run. You should define it if there is cleanup work to do.
    virtual void TearDown() {
      if (mThreadPool) {
        mThreadPool.release();
      }
    }

    std::unique_ptr<ThreadPool> mThreadPool;
  };

}

#endif /* B1CC2146_6FBC_4BA6_855C_625AA394500A */
