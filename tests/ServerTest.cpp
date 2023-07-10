/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-07 12:00:00                              *
 * @LastEditDate          : 2023-07-10 15:06:38                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "ServerTest.hpp"

#include "../Agent.hpp"
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
    agentShortLife.reset();

    std::this_thread::sleep_for(1000ms);
    agentLongLife.reset();

    //TODO : add interfaces to check the arrival of the TICK messages
  }

}