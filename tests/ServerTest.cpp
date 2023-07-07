#include "ServerTest.hpp"
#include "../Logger.hpp"


namespace Cmder::Testing {

  Server ServerTest::sServer;

  TEST_F(ServerTest, StartStop)
  {
    sServer.start();
    sServer.stop();
  }
}