/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-11 11:17:28                              *
 * @LastEditDate          : 2023-07-11 11:20:10                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#ifndef FED2C17D_5272_4374_9F47_1EACE7280CF1
#define FED2C17D_5272_4374_9F47_1EACE7280CF1

#include <cstdint>
#include <string>

namespace Cmder {

  class IStatProvider {
  public:
    virtual uint64_t statNotification(const std::string& notificationName) const = 0;
  };

}

#endif /* FED2C17D_5272_4374_9F47_1EACE7280CF1 */
