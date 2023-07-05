
#include "Agent.hpp"
#include "Callback.hpp"
#include "Logger.hpp"
#include "Server.hpp"

// For testing:
#include "tests/Test.hpp"


#include <iostream>

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