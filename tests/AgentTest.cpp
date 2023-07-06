
#include "AgentTest.hpp"
#include "../Agent.hpp"

namespace Cmder::Testing {

  Server AgentTest::sServer;
  Callback::SharedPtr AgentTest::sCallback;
  Agent::SharedPtr AgentTest::sNoCallbackAgent;
  Agent::SharedPtr AgentTest::sAsyncAgent;

  TEST_F(AgentTest, AsyncAgent_BlockingCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    ASSERT_TRUE(sCallback);
    std::string result;
    Receipt receipt = sAsyncAgent->doTask(Receipt::Blocking, Agent::Pi, result);

    EXPECT_EQ(result, "3.14");

    EXPECT_GT(receipt.getTaskId(), 0); 
    EXPECT_EQ(receipt.getStatus(), Receipt::OK); 
    EXPECT_EQ(receipt.getExecutionMode(), Receipt::Blocking);

    std::optional<Callback::Message_t> message = sCallback->getFirst(receipt, Callback::RESULT);
    EXPECT_FALSE(message.has_value());
  }

  TEST_F(AgentTest, AsyncAgent_AsyncCall)
  {
    ASSERT_TRUE(sAsyncAgent);
    std::string result;
    Receipt receipt = sAsyncAgent->doTask(Receipt::Blocking, Agent::Pi, result);

//    EXPECT_EQ(result, "3.14");

//    EXPECT_GT(receipt.getTaskId(), 0); 
//    EXPECT_EQ(receipt.getStatus(), Receipt::OK); 
//    EXPECT_EQ(receipt.getExecutionMode(), Receipt::Blocking); 
  }

}