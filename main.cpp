
#include <iostream>
#include "Logger.hpp"

#include "Agent.hpp"

#include "Server.hpp"

int main() {

  Server server;
  server.start();

  pthread_setname_np(pthread_self(), "TH_MAIN");

  LOG("Program start");

  {
    Agent::SharedPtr agentSp = Agent::create();
    if (agentSp) {
      agentSp->doTask(Agent::Blocking, "???");
    }
  }

  LOG("Program end");
  return 0;
}