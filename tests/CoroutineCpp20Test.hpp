#ifndef C9884684_4952_4E3B_8020_C3B02A559AC2
#define C9884684_4952_4E3B_8020_C3B02A559AC2

#include <gtest/gtest.h>


namespace cmder::tst {


  class CoroutineCpp20Test : public testing::Test {
  protected:  // You should make the members protected s.t. they can be accessed from sub-classes.
        
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite()
    {
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
    }

    // virtual void SetUp() will be called before each test is run.
    void SetUp() override
    {
    }

    // virtual void TearDown() will be called after each test is run. You should define it if there is cleanup work to do.
    //virtual void TearDown() {
    //}
  };
}

#endif /* C9884684_4952_4E3B_8020_C3B02A559AC2 */
