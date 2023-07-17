/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-07-06 12:00:00                               *
 * @LastEditDate          : 2023-07-17 21:52:18                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/


#include "AgentTest.hpp"
#include <agent/Agent.hpp>
#include <agent/Task.hpp>
#include <server/Server.hpp>

namespace cmder::tst {

  Server AgentTest::sServer;
  Callback::SharedPtr AgentTest::sCallback;
  Agent::SharedPtr AgentTest::sNoCallbackAgent;
  Agent::SharedPtr AgentTest::sAsyncAgent;

  TEST_F(AgentTest, AsyncAgent_BlockingCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    ASSERT_TRUE(sCallback);
    std::string result;
    Receipt receipt = sAsyncAgent->doTask(ExecutionMode_t::Blocking, TaskName::Pi, result);

    EXPECT_EQ(result, "3.14");

    EXPECT_GE(receipt.getTaskId(), 0); 
    EXPECT_EQ(receipt.getStatus(), Receipt::OK); 
    EXPECT_EQ(receipt.getExecutionMode(), ExecutionMode_t::Blocking);

    // Don't have any RESULT in the callback. Result was given as the return value of the blocking doCmd()
    EXPECT_LT(0, sCallback->messagesSize());
    while (sCallback->messagesSize()) {
      std::optional<Callback::Message_t> message = sCallback->tryPop();
      EXPECT_TRUE(message.has_value());
      EXPECT_EQ(message->mType, CallbackMessageType_t::NOTIFICATION);
    }
    EXPECT_EQ(sCallback->messagesSize(), 0);

    std::optional<Callback::Message_t> message = sCallback->tryPop();
    EXPECT_FALSE(message.has_value());
  }

  TEST_F(AgentTest, AsyncAgent_AsyncCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    ASSERT_TRUE(sCallback);
    std::string result("No result yet");
    Receipt receipt = sAsyncAgent->doTask(ExecutionMode_t::Async, TaskName::Pi, result);

    EXPECT_EQ(result, "");

    EXPECT_NE(receipt.getTaskId(), INVALID_TASK_ID);
    EXPECT_NE(receipt.getTaskId(), NO_TASK_ID);
    EXPECT_EQ(receipt.getStatus(), Receipt::OK);
    EXPECT_EQ(receipt.getExecutionMode(), ExecutionMode_t::Async);

    // RESULT is in the callback.
    bool hasResult = false;
    while ( ! hasResult) {
      std::optional<Callback::Message_t> message = sCallback->pop();
      EXPECT_TRUE(message.has_value());

      if (message->mType == CallbackMessageType_t::RESULT) {
        hasResult = true;
      }
      EXPECT_EQ(message->mTaskId, receipt.getTaskId());
    }
    EXPECT_EQ(sCallback->messagesSize(), 0);

    std::optional<Callback::Message_t> message = sCallback->tryPop();
    EXPECT_FALSE(message.has_value());
  }

  TEST_F(AgentTest, AsyncNoCallback)
  {
    //std::string result;
    //EXPECT_AS(sNoCallbackAgent->doTask(Receipt::Async, Agent::Pi, result), "Cannot call async task execution");
  }

}