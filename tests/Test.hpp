#ifndef SERVERTEST_H_INCLUDED
#define SERVERTEST_H_INCLUDED

#include "../Logger.hpp"

//  mingw-w64-x86_64-gtest !
#include <gtest/gtest.h>

namespace {

  void runTests()
  {
    testing::InitGoogleTest();
    std::cout << std::endl;
    int retVal =  RUN_ALL_TESTS();
    LOG("Tests executed: " << retVal);
  }


  // To use gtest fixture, derive from testing::Test 
  class ServerTest : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
    
    // virtual void SetUp() will be called before each test is run.
    void SetUp() override {
      // TODO
    }

    // virtual void TearDown() will be called after each test is run. You should define it if there is cleanup work to do.
    //virtual void TearDown() {
    //}

  };
}

#endif