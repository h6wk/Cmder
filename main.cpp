/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-06-29 12:00:00                               *
 * @LastEditDate          : 2023-07-09 23:19:24                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/


#include "Agent.hpp"
#include "Callback.hpp"
#include "Logger.hpp"
#include "Server.hpp"

// For testing:
#include "tests/ServerTest.hpp"


#include <iostream>


using namespace Cmder;


int main() {

  LOG("Program start");

  Server server;
  server.start();

  {
    Callback::SharedPtr callback = std::make_shared<Callback>();
    Agent::SharedPtr agentSp = Agent::create(server, callback);
    
    if (agentSp) {
      std::string result;
      const Receipt receipt = agentSp->doTask(Receipt::Blocking, Agent::Pi, result);
    }

    LOG("Notifications: " << callback);
  }

  LOG("Program end");

  // For testing:
  runTests();

  return 0;
}