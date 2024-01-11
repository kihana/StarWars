#include "exception_handler.h"

#include <format>
#include <stdexcept>

namespace server::commands {

void ExceptionHandler::Handle(const std::exception& exception, const std::shared_ptr<Command>& command) {
  std::size_t seed = 0;
  std::string_view type_name = typeid(exception).name();
  boost::hash_combine(seed, type_name);
  type_name = typeid(*command).name();
  boost::hash_combine(seed, type_name);

  const auto it = handlers_.find(seed);
  if (handlers_.cend() == it)
    throw std::runtime_error(std::format("Failed to find '{}' exception handler for '{}' command.",
                                         typeid(exception).name(), typeid(command).name()));

  // TODO(Maverick): call handler if formal argument types are base types for actual argument types.
  it->second(exception, command);
}

} // namespace server::commands
