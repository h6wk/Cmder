#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <condition_variable>
#include <memory>
#include <thread>

class Server {
public:
  Server();
  virtual ~Server();

  void start();

  void stop();

private:
  void run();

  std::unique_ptr<std::thread> mThreadPtr;

  std::mutex mMutex;
  std::condition_variable mConditionVariable;

  enum Status {Init, Start, Run, Stop};
  Status mStatus;
};

#endif
