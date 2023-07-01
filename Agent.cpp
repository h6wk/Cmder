
#include "Agent.hpp"
#include "Logger.hpp"

Agent::SharedPtr Agent::create()
{
  //Use a temporary subclass to make a connection between a smart pointer generator function and a class with a private constructor.
  struct MkSharedEnabler : public Agent {};
  auto instance = std::make_shared<MkSharedEnabler>();
  return instance;
}

Agent::Agent()
{
  LOG("Created");
}

Agent::~Agent()
{
  LOG("Deleted");
}
