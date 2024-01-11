#pragma once

#include <exception>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <cstddef>

#include <boost/functional/hash.hpp>

#include "callable.h"
#include "command.h"

namespace server::commands {

class ExceptionHandler {
public:
  ExceptionHandler(const ExceptionHandler& that) = delete;
  ExceptionHandler& operator=(const ExceptionHandler& that) = delete;

  static ExceptionHandler& Instance() {
    static ExceptionHandler instance;

    return instance;
  }

  // TODO(Maverick): deduce parameters from Func parameter to get rid of specification particular types explicitly.
  template <typename TException, typename TCommand, typename Func> void AddHandler(Func&& func) {
    std::size_t seed = 0;
    std::string_view type_name = typeid(TException).name();
    boost::hash_combine(seed, type_name);
    type_name = typeid(TCommand).name();
    boost::hash_combine(seed, type_name);

    handlers_.insert_or_assign(seed, sw::Callable<void>(std::forward<Func>(func)));
  }

  void Handle(const std::exception& exception, const std::shared_ptr<Command>& command);

private:
  ExceptionHandler() = default;
  ~ExceptionHandler() = default;

private:
  std::unordered_map<std::size_t, sw::Callable<void>> handlers_;
};

} // namespace server::commands