#include "command.h"

#include "core/object.h"

namespace server::commands {

Adapter::Adapter(const std::shared_ptr<core::Object>& property_holder) : property_holder_(property_holder) {
}

std::any& Adapter::GetAnyValue(std::string_view property_name) {
  return GetAnyValueImpl(*this, property_name);
}

const std::any& Adapter::GetAnyValue(std::string_view property_name) const {
  return GetAnyValueImpl(*this, property_name);
}

InjectableCommand::InjectableCommand(std::unique_ptr<Command> command) : command_(std::move(command)) {
}

void InjectableCommand::Inject(std::unique_ptr<Command> command) {
  command_ = std::move(command);
}

void InjectableCommand::Execute() {
  command_->Execute();
}

const std::unique_ptr<Command>& InjectableCommand::GetCommand() const noexcept {
  return command_;
}

void NopCommand::Execute() {
}

} // namespace server::commands