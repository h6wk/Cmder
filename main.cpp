
#include "Agent.hpp"
#include "Callback.hpp"
#include "Logger.hpp"
#include "Server.hpp"

#include <iostream>

int main() {

  LOG("Program start");

  Server server;
  server.start();

  {
    Callback::SharedPtr callback = std::make_shared<Callback>();
    Agent::SharedPtr agentSp = Agent::create(server, callback);
    
    if (agentSp) {
      agentSp->doTask(Agent::Blocking, "???");
    }
  }

  LOG("Program end");
  return 0;
}