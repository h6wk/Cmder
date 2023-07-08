#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <condition_variable>
#include <memory>
#include <thread>

class Server {
public:
  enum Status {Init, Start, Run, Stop};
  
  Server();
  virtual ~Server();

  void start();

  void stop();

  /// @brief Get back the status value
  /// @return Enum value
  Status getStatus() const;

private:
  void run();

  std::unique_ptr<std::thread> mThreadPtr;

  mutable std::mutex mMutex;
  std::condition_variable mConditionVariable;

  Status mStatus;
};

#endif
