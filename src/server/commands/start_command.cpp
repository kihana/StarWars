#include "start_command.h"

#include <format>
#include <stdexcept>

namespace server::commands {

StartCommandAdapter::StartCommandAdapter(const std::shared_ptr<core::Object>& order) : Adapter(order) {
}

StartCommand::StartCommand(std::unique_ptr<StartCommandAdapter> adapter) : adapter_(std::move(adapter)) {
}

void StartCommand::Execute() {
  if (!adapter_)
    throw std::runtime_error(std::format("'{}' has not been initialized.", kStartCommandName));

  adapter_->DoStartAction();
}

} // namespace server::commands
