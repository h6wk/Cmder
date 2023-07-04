#include "Receipt.hpp"

Receipt::Receipt()
: mTaskId(0)
, mStartTime(std::chrono::system_clock::now())
{
}