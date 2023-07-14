/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-06 12:00:00                              *
 * @LastEditDate          : 2023-07-13 15:01:52                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef C24E3114_7C87_43CB_87F1_FA599D88BA16
#define C24E3114_7C87_43CB_87F1_FA599D88BA16

#include <gtest/gtest.h>

#include <agent/Agent.hpp>
#include <server/Server.hpp>

namespace cmder::tst {

  using cmder::srv::Server;
  using namespace cmder::agent;

  class AgentTest : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
        
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite()
    {
      sCallback = std::make_shared<Callback>();
      sNoCallbackAgent = Agent::create(sServer, Callback::SharedPtr());
      sAsyncAgent = Agent::create(sServer, sCallback);
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
      sAsyncAgent.reset();
      sNoCallbackAgent.reset();
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

    static Server sServer;
    static Callback::SharedPtr sCallback;
    static Agent::SharedPtr sNoCallbackAgent;
    static Agent::SharedPtr sAsyncAgent;
  };
}

#endif /* C24E3114_7C87_43CB_87F1_FA599D88BA16 */
