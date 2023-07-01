
#include <iostream>
#include "Logger.hpp"

#include "Agent.hpp"

int main() {
  pthread_setname_np(pthread_self(), "TH_MAIN");

  LOG("Start");

  Agent agent;

  LOG("End");
  return 0;
}