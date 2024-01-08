#include "end_command.h"

#include <format>
#include <stdexcept>

namespace server::commands {

EndCommandAdapter::EndCommandAdapter(const std::shared_ptr<core::Object>& order) : Adapter(order) {
}

EndCommand::EndCommand(std::unique_ptr<EndCommandAdapter> adapter) : adapter_(std::move(adapter)) {
}

void EndCommand::Execute() {
  if (!adapter_)
    throw std::runtime_error(std::format("'{}' has not been initialized.", kEndCommandName));

  adapter_->DoEndAction();
}

} // namespace server::commands
