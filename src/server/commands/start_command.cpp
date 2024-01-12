#include "start_command.h"

#include <format>

#include "exception.h"

namespace server::commands {

StartCommandAdapter::StartCommandAdapter(const std::shared_ptr<core::Object>& order) : Adapter(order) {
}

StartCommand::StartCommand(std::unique_ptr<StartCommandAdapter> adapter) : adapter_(std::move(adapter)) {
}

void StartCommand::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' has not been initialized.", kStartCommandName));

  adapter_->DoStartAction();
}

} // namespace server::commands
