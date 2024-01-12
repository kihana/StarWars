#include "repeat.h"

#include <format>

namespace server::commands {

RepeatAdapter::RepeatAdapter(const std::shared_ptr<core::Object>& repeatable) : Adapter(repeatable) {
}

const std::shared_ptr<Command>& RepeatAdapter::GetRepeatCommand() const {
  const auto& any_command = GetAnyValue(kCommandName);

  return CastAnyRefToRef<std::shared_ptr<Command>>(kCommandName, any_command);
}

Queue<std::shared_ptr<Command>>& RepeatAdapter::GetCommandQueue() const {
  auto& any_command_queue = GetAnyValue(kCommandQueueName);

  return CastAnyPointerToRef<Queue<std::shared_ptr<Command>>>(kCommandQueueName, any_command_queue);
}

std::string_view RepeatAdapter::GetAdapterName() const {
  return kRepeatAdapterName;
}

std::string_view RepeatAdapter::GetParentAdapterName() const {
  return kRepeatCommandName;
}

Repeat::Repeat(const std::shared_ptr<Repeatable>& adapter) : adapter_(adapter) {
}

void Repeat::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kRepeatAdapterName));

  auto& command_to_repeat = adapter_->GetRepeatCommand();
  command_to_repeat->Execute();
}

} // namespace server::commands