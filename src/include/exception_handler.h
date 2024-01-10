#pragma once

#include <format>
#include <functional>
#include <stdexcept>
#include <string_view>
#include <typeinfo>
#include <unordered_map>

#include "callable.h"

namespace server::commands {

class ExceptionHandler {
public:
  ExceptionHandler(const ExceptionHandler& that) = delete;
  ExceptionHandler& operator=(const ExceptionHandler& that) = delete;

  static ExceptionHandler& Instance() {
    static ExceptionHandler instance;

    return instance;
  }

  // TODO(Maverick): deduce Args parameters from Func parameter to get rid of specification Args types explicitly.
  template <typename... Args, typename Func> void AddHandler(Func&& func) {
    using ArgsTuple = std::tuple<Args...>;
    static const std::string_view type = typeid(ArgsTuple).name();

    handlers_.emplace(type, sw::Callable<void>(std::forward<Func>(func)));
  }

  template <typename... Args> void Handle(Args&&... args) {
    using ArgsTuple = std::tuple<Args...>;
    static const std::string_view type = typeid(ArgsTuple).name();

    const auto it = handlers_.find(type);
    if (handlers_.cend() == it)
      throw std::runtime_error(std::format("Failed to find exception handler for '{}' parameters.", type));

    // TODO(Maverick): call handler if formal argument types are base types for actual argument types.
    it->second(std::forward<Args>(args)...);
  }

private:
  ExceptionHandler() = default;
  ~ExceptionHandler() = default;

private:
  std::unordered_map<std::string_view, sw::Callable<void>> handlers_;
};

} // namespace server::commands