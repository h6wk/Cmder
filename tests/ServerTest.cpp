/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-07 12:00:00                              *
 * @LastEditDate          : 2023-07-11 23:39:06                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "ServerTest.hpp"

#include <agent/Agent.hpp>
#include "../IControllableThread.hpp"
#include "../Logger.hpp"


namespace Cmder::Testing {

  using namespace std::chrono_literals;

  TEST_F(ServerTest, StartStop)
  {
    EXPECT_EQ(mServer->getStatus(), Status::Init);
    mServer->start();
    EXPECT_TRUE(mServer->getStatus() == Status::Start || mServer->getStatus() == Status::Run);
    mServer->stop();
    EXPECT_EQ(mServer->getStatus(), Status::Stop);
  }

  TEST_F(ServerTest, RegisterAgentAndStop)
  {
    mServer->start();

    Agent::SharedPtr agentShortLife = Agent::create(*mServer, Callback::SharedPtr());
    Agent::SharedPtr agentLongLife = Agent::create(*mServer, Callback::SharedPtr());

    std::this_thread::sleep_for(1000ms);
    agentShortLife->unregisterAgent();

    std::this_thread::sleep_for(1000ms);
    agentLongLife->unregisterAgent();

    EXPECT_GT(agentLongLife->statNotification("TICK"), agentShortLife->statNotification("TICK"));
    EXPECT_EQ(mServer->statNotification("TICK"), agentShortLife->statNotification("TICK") + agentLongLife->statNotification("TICK"));
  }

}