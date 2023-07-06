#include "Test.hpp"
#include "../Logger.hpp"

#include <iostream>


////////////////////////// SIMPLE TEST
TEST(FirstTest, TestOne)
{
  EXPECT_TRUE(true);
}



/////////////////////////// FIXTURES
TEST_F(ServerTest, T1)
{
  //EXPECT_TRUE(false);
}