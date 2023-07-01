
#include <thread>
#include <memory>

class Server {
public:
  Server();
  virtual ~Server();

  void start();

  void stop();

private:
  void run();

  std::unique_ptr<std::thread> mThreadPtr;
};
