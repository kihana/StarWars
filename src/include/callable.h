#pragma once

#include <any>
#include <functional>
#include <utility>

namespace sw {

template <typename TReturn> struct Callable {
  Callable() {
  }

  template <typename TFunc> Callable(TFunc&& func) : Callable(std::function(std::forward<TFunc>(func))) {
  }

  template <typename... Args> Callable(std::function<TReturn(Args...)> func) : callable_(func) {
  }

  template <typename... Args> TReturn operator()(Args&&... args) {
    return std::invoke(std::any_cast<std::function<TReturn(Args...)>>(callable_), std::forward<Args>(args)...);
  }

  std::any callable_;
};

} // namespace sw
