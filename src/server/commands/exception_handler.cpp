#include "exception_handler.h"

#include <format>
#include <stdexcept>

namespace server::commands {

void ExceptionHandler::Handle(const std::exception& exception, const std::shared_ptr<Command>& command) {
  std::size_t seed = 0;
  std::string_view exception_type_name = typeid(exception).name();
  boost::hash_combine(seed, exception_type_name);
  std::string_view command_type_name = typeid(*command).name();
  boost::hash_combine(seed, command_type_name);

  auto it = handlers_.find(seed);
  if (handlers_.cend() == it) {
    std::string_view base_exception_type_name = typeid(std::exception).name();
    if (exception_type_name != base_exception_type_name) {
      seed = 0;
      boost::hash_combine(seed, base_exception_type_name);
      boost::hash_combine(seed, command_type_name);
      it = handlers_.find(seed);
    }
  }

  if (handlers_.cend() == it) {
    throw std::runtime_error(std::format("Failed to find '{}' exception handler for '{}' command.",
                                         typeid(exception).name(), typeid(command).name()));
  }

  // TODO(Maverick): call handler if formal argument types are base types for actual argument types.
  it->second(exception, command);
}

void ExceptionHandler::Clean() {
  handlers_.clear();
}

} // namespace server::commands
