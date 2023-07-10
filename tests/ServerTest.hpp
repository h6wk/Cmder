/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-07 12:00:00                              *
 * @LastEditDate          : 2023-07-10 14:04:24                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef SERVERTEST_H_INCLUDED
#define SERVERTEST_H_INCLUDED

#include "../Logger.hpp"
#include "../Server.hpp"

//  mingw-w64-x86_64-gtest !
#include <gtest/gtest.h>

namespace {
  void runTests()
  {
    testing::InitGoogleTest();
    std::cout << std::endl;
    int retVal =  RUN_ALL_TESTS();
    LOG("Tests executed: " << retVal);
  }
}

namespace Cmder::Testing {

  // To use gtest fixture, derive from testing::Test 
  class ServerTest : public testing::Test {
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
      mServer = std::make_unique<Server>();
    }

    // virtual void TearDown() will be called after each test is run. You should define it if there is cleanup work to do.
    virtual void TearDown() {
      if (mServer) {
        mServer.release();
      }
    }


    std::unique_ptr<Server> mServer;
  };
}

#endif