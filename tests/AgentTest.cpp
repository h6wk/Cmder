/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-06 12:00:00                              *
 * @LastEditDate          : 2023-07-13 15:02:16                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/


#include "AgentTest.hpp"
#include <agent/Agent.hpp>
#include <agent/Task.hpp>
#include <server/Server.hpp>

namespace cmder::tst {

  using cmder::Server;

  Server AgentTest::sServer;
  Callback::SharedPtr AgentTest::sCallback;
  Agent::SharedPtr AgentTest::sNoCallbackAgent;
  Agent::SharedPtr AgentTest::sAsyncAgent;

  TEST_F(AgentTest, AsyncAgent_BlockingCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    ASSERT_TRUE(sCallback);
    std::string result;
    Receipt receipt = sAsyncAgent->doTask(Receipt::Blocking, TaskName::Pi, result);

    EXPECT_EQ(result, "3.14");

    EXPECT_GE(receipt.getTaskId(), 0); 
    EXPECT_EQ(receipt.getStatus(), Receipt::OK); 
    EXPECT_EQ(receipt.getExecutionMode(), Receipt::Blocking);

    // Don't have any RESULT in the callback. Result was given as the return value of the blocking doCmd()
    std::optional<Callback::Message_t> message = sCallback->getFirst(receipt, Callback::RESULT);
    EXPECT_FALSE(message.has_value());
  }

  TEST_F(AgentTest, AsyncAgent_AsyncCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    ASSERT_TRUE(sCallback);
    std::string result("No result yet");
    Receipt receipt = sAsyncAgent->doTask(Receipt::Async, TaskName::Pi, result);

    EXPECT_EQ(result, "");

    EXPECT_GT(receipt.getTaskId(), 0);
    EXPECT_EQ(receipt.getStatus(), Receipt::OK);
    EXPECT_EQ(receipt.getExecutionMode(), Receipt::Async);

    // RESULT is in the callback.
    std::optional<Callback::Message_t> message = sCallback->waitFirst(receipt, Callback::RESULT);
    EXPECT_TRUE(message.has_value());
    EXPECT_EQ(message->mResult, "3.14");
  }

  TEST_F(AgentTest, AsyncNoCallback)
  {
    //std::string result;
    //EXPECT_AS(sNoCallbackAgent->doTask(Receipt::Async, Agent::Pi, result), "Cannot call async task execution");
  }

}