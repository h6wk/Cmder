/******************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                           *
 * @CreatedDate           : 2023-06-29 12:00:00                               *
 * @LastEditDate          : 2023-07-18 11:26:10                               *
 * @CopyRight             : GNU GPL                                           *
 *****************************************************************************/


#include "Logger.hpp"

// For testing:
#include "../tests/ServerTest.hpp"

#include <iostream>


int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  std::cout << std::endl;
  int retVal =  RUN_ALL_TESTS();
  LOG("Tests executed");

  return retVal;
}