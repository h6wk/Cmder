#ifndef AGENTTEST_H_INCLUDED
#define AGENTTEST_H_INCLUDED

#include <gtest/gtest.h>

#include "../Agent.hpp"

namespace Cmder::Testing {
  class AgentTest : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
        
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite() {
      sCallback = std::make_shared<Callback>();
      sNoCallbackAgent = Agent::create(sServer, Callback::SharedPtr());
      sAsyncAgent = Agent::create(sServer, sCallback);
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() {
      sAsyncAgent.reset();
      sNoCallbackAgent.reset();
      sCallback.reset();
    }

    // virtual void SetUp() will be called before each test is run.
    void SetUp() override {
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

#endif