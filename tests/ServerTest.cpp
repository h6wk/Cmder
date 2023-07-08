#include "ServerTest.hpp"
#include "../Logger.hpp"


namespace Cmder::Testing {

  Server ServerTest::sServer;

  TEST_F(ServerTest, StartStop)
  {
    EXPECT_EQ(sServer.getStatus(), Server::Init);
    sServer.start();
    EXPECT_TRUE(sServer.getStatus() == Server::Start || sServer.getStatus() == Server::Run);
    sServer.stop();
    EXPECT_EQ(sServer.getStatus(), Server::Stop);
  }
}