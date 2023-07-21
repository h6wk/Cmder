/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-19 21:48:49                              *
 * @LastEditDate          : 2023-07-21 11:51:26                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "CoroutineCpp20Test.hpp"

#include <Logger.hpp>

#include <coroutine>

/////////////////////////////////////////////////////// DEFINITIONS
/// Task: Coroutine that does a job without returning a value
/// Generator: Coroutine that does a job and returns a value
///




/////////////////////////////////////////////////////// EXAMPLE 1

namespace {

  /// @brief Coroutine generator skeleton
  /// @tparam T 
  template<typename T>
  struct Generator {
  public:
    struct promise_type {
    public:
      /// @brief Return Generator with a reference of promise_type
      /// Example: foo() will end up with a Generator{*this}
      ///   Generator<std::string> foo;
      ///   const auto f = foo();
      auto get_return_object() noexcept
      {
        return Generator{ *this };
      }

      ///
      /// @brief Returns an awaitable object.
      ///
      /// Example:
      ///   Generator<std::string> foo() {
      ///     <<<<< Initial suspend here! >>>>>
      ///     co_yield "hello";
      ///     ..
      ///     co_yield "blabla";
      ///     <<<<< Final suspend here!   >>>>>
      ///   }
      std::suspend_always initial_suspend() const noexcept
      {
        return {};
      }
      std::suspend_always final_suspend() const noexcept
      {
        return {};
      }

      /// @brief Store the result that is given to the class
      ///
      /// Example:
      ///   being called after co_yield expression (co_yield expr === co_await promise.yield_value(expr))
      std::suspend_always yield_value(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
      {
        mResult = value;
        return {};
      }

      /// @brief 
      /// Example:
      ///    being called at co_return;
      void return_void() const noexcept
      {
      }

      /// @brief Store the exception in our variant
      /// Example:
      ///   try {
      ///     // function body
      ///   }
      ///   catch(...) {}
      ///     promise.undhandled_exception();
      ///   }
      void unhandled_exception() noexcept(std::is_nothrow_copy_constructible_v<std::exception_ptr>)
      {
        mResult = std::current_exception();
      }

      /// @brief Must have a result or exception (precondition)
      T& getValue()
      {
        if (std::holds_alternative<std::exception_ptr>(mResult)) {
          std::rethrow_exception(std::get<std::exception_ptr>(mResult));
        }
        return std::get<T>(mResult);
      }

    private:
      std::variant<std::monostate, T, std::exception_ptr> mResult;
    };

    Generator(Generator&& other) noexcept
    : mCoro{std::exchange(other.mCoro, nullptr)}
    {
    }

    Generator& operator=(Generator&& other) noexcept
    {
      if (mCoro) {
        mCoro.destroy();
      }
      mCoro = std::exchange(other.mCoro, nullptr);
    }

    ~Generator()
    {
      if (mCoro) {
        mCoro.destroy();
      }      
    }

    auto& operator()() const
    {
      mCoro(); // same as mCoro.resume();
      return mCoro.promise().getValue();
    }

  private:
    explicit Generator(promise_type& promise) noexcept
    : mCoro{ std::coroutine_handle<promise_type>::from_promise(promise) }
    {
    }

    std::coroutine_handle<promise_type> mCoro;
  };
}


Generator<std::string> foo()
{
  co_yield "hello";
  co_yield "world";
  co_return;          // <---- called implicitely if it was missing
}

namespace cmder::tst {

  TEST_F(CoroutineCpp20Test, GeneratorHelloWorldExample)
  {
    const auto f = foo();
    LOG(f() << " " << f());
  }
}


/////////////////////////////////////////////////////// EXAMPLE 2
// Author: C++20’s Coroutines for Beginners - Andreas Fertig - CppCon 2022

struct Chat {

  struct promise_type {
    std::string mMsgOut{};        // <--- storing a value from the coroutine
    std::string mMsgIn{};         // <--- storing a value to the coroutine

    void unhandled_exception() noexcept {}                        // <---- what to do in case of unhandled exception
    
    Chat get_return_object() noexcept { return Chat{this};}      // <---- coroutine creation

    std::suspend_always initial_suspend() noexcept { return {}; } // <---- startup

    std::suspend_always yield_value(std::string msg) noexcept     // <---- value from co_yield
    {
        mMsgOut = std::move(msg);
        return {};
    }

    auto await_transform(std::string msg) noexcept                // <---- value from co_await
    {
        struct awaiter {        // <----- customized version instead of using suspend_always or suspend_never
        promise_type& mPt;
        constexpr bool await_ready() const noexcept { return true; }
        std::string await_resume() const noexcept { return std::move(mPt.mMsgIn); }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
        };
        return awaiter{*this};
    }

    void return_value(std::string msg) noexcept { mMsgOut = std::move(msg); }    // <------ value from co_return

    std::suspend_always final_suspend() noexcept { return {}; } // <---- ending
  };


  using Handle = std::coroutine_handle<promise_type>;       // <------ shortcut to the type
  Handle mCoroHandle;

  explicit Chat(promise_type *p)                            // <------ get the handle from the promise
  : mCoroHandle{Handle::from_promise(*p)}
  {}

  Chat(Chat&& other)
  : mCoroHandle{std::exchange(other.mCoroHandle, nullptr)}
  {}

  ~Chat()
  {
    if (mCoroHandle) {mCoroHandle.destroy();}
  }

  std::string listen()          // <----- activate the coroutine and wait for data
  {
    if (not mCoroHandle.done()) { 
      mCoroHandle.resume();
    }
    return std::move(mCoroHandle.promise().mMsgOut);
  }

  void answer(std::string msg)  // <------ send data to the coroutine and activate it
  {
    mCoroHandle.promise().mMsgIn = std::move(msg);
    if (not mCoroHandle.done()) {
      mCoroHandle.resume();
    }
  }
};

Chat fun()                                  // <---- "wrapper type" Chat must contain the promise_type
{
    co_yield "hello\n";                     // <---- calls the promise_type.yield_value
    std::cout << co_await std::string{};    // <---- calls the promise_type.await_transform
    co_return "here\n";                     // <---- calls the promise_type.return_value
}

namespace cmder::tst {
  TEST_F(CoroutineCpp20Test, HelloWorldDuplexComm)
  {
    Chat chat = fun();                      // <---- creation of the coroutine
    std::cout << chat.listen();             // <---- trigger the machine
    chat.answer("where are you?\n");        // <---- sends data into the coroutine
    std::cout << chat.listen();             // <---- wait more data from the coroutine
  }
}

/////////////////////////////////////////////////////// EXAMPLE 3 (Interleave two std::vector s)
// Author: C++20’s Coroutines for Beginners - Andreas Fertig - CppCon 2022


struct GeneratorInterleave {

  struct promise_type {
    int mVal{};

    GeneratorInterleave get_return_object() noexcept { 
      LOG("");
      return GeneratorInterleave{this};
    }
    
    std::suspend_never initial_suspend() noexcept {     // <---- startup
      LOG("");
      return {};
    }
    std::suspend_always final_suspend() noexcept {      // <---- ending
      LOG("");
      return {};
    }

    std::suspend_always yield_value(int v) noexcept     // <---- value from co_yield
    {
      LOG("value=" << v);
      mVal = v;
      return {};
    }

    void unhandled_exception() noexcept {
      LOG("");
    }
  };

  using Handle = std::coroutine_handle<promise_type>;       // <------ shortcut to the type
  Handle mCoroHandle;

  explicit GeneratorInterleave(promise_type *p)                            // <------ get the handle from the promise
  : mCoroHandle{Handle::from_promise(*p)}
  {
    LOG("");
  }

  GeneratorInterleave(GeneratorInterleave&& other)
  : mCoroHandle{std::exchange(other.mCoroHandle, nullptr)}
  {
    LOG("");
  }

  ~GeneratorInterleave()
  {
    LOG("");
    if (mCoroHandle) {mCoroHandle.destroy();}
  }

  int value() const
  {
    LOG("");
    return mCoroHandle.promise().mVal;
  }

  bool finished()
  {
    LOG("");
    return mCoroHandle.done();
  }

  void resume()
  {
    LOG("");
    if (not finished()) {
      mCoroHandle.resume();
    }
  }

};


GeneratorInterleave interleaved(std::vector<int> a, std::vector<int> b)
{
  LOG("");
  auto lamb = [](std::vector<int>& v) -> GeneratorInterleave {
    for (const auto& e : v) {
      LOG("co_yield " << e);
      co_yield e;
    }
  };

  LOG("");
  auto x = lamb(a);
  LOG("");
  auto y = lamb(b);
  LOG("");


  while (not x.finished() or not y.finished()) {
    LOG("");
    if (not x.finished()) {
      LOG("");
      co_yield x.value();
      LOG("");
      x.resume();
    }

    if (not y.finished()) {
      LOG("");
      co_yield y.value();
      LOG("");
      y.resume();
    }
  }
}

namespace cmder::tst {
  TEST_F(CoroutineCpp20Test, VectorInterleave)
  {
    std::vector a{1,3,5,7,9};
    std::vector b{2,4,6};

    LOG("");
    GeneratorInterleave g{interleaved(std::move(a), std::move(b))};
    LOG("");
    while (not g.finished()) {
      LOG("");
      std::cout << g.value() << std::endl;
      LOG("");
      g.resume();
    }
  }
}