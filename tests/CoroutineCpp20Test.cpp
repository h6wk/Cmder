/*****************************************************************************
 * @Author                : h6wk<h6wking@gmail.com>                          *
 * @CreatedDate           : 2023-07-19 21:48:49                              *
 * @LastEditDate          : 2023-07-19 21:55:31                              *
 * @CopyRight             : GNU GPL                                          *
 ****************************************************************************/

#include "CoroutineCpp20Test.hpp"

#include <Logger.hpp>

#include <coroutine>

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
