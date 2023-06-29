
#include <iostream>
#include "Logger.hpp"

int main() {
  pthread_setname_np(pthread_self(), "TH_MAIN");

  LOG("Start");
  LOG("End");
  return 0;
}