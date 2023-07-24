/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-22 22:34:22                              *
 * @LastEditDate          : 2023-07-24 09:00:02                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include <tools/ThreadPool.hpp>

#include <Logger.hpp>

namespace cmder::tools {

  ThreadPool::ThreadPool(size_t threadCount)
  {    
    for (size_t i = 0; i < threadCount; ++i) {
      LOG("Thread " << i+1 << " creation");

      std::thread worker([this]() {
        while (true) {

          std::function<void()> task;

          { // mutex scope start
            std::unique_lock lock(mMutex);
            LOG("Worker thread in the loop");
            mConditionVariable.wait(
              lock
              , [this]() { return mStopThreadPool || not mTasks.empty(); });
            LOG("Worker thread in the loop signalled or woke up");

            if (mStopThreadPool && mTasks.empty()) {
              LOG("Worker thread in the loop leaves the loop");
              return;
            }

            task = std::move(mTasks.front());
            mTasks.pop();
          } // mutex scope ends

          // Mutex must be unlocked!
          LOG("Worker thread in the loop executes task");
          task();
        }
      });

      mThreads.emplace_back(std::move(worker));
      LOG("Worker thread " << i+1 << " moved in pool's container");
    }
  }

  ThreadPool::~ThreadPool()
  {
    LOG("");

    /* stop thread pool, and notify all threads to finish the remained tasks. */
    {
      std::unique_lock<std::mutex> lock(mMutex);
      mStopThreadPool = true;
    }

    mConditionVariable.notify_all();
    for (auto &worker : mThreads) {
      LOG("Worker thread going to be joined");
      worker.join();
    }
  }
}