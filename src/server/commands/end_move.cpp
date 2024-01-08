#include "end_move.h"

#include <format>
#include <stdexcept>

#include "move.h"
#include "set_velocity.h"

namespace server::commands {

EndMoveAdapter::EndMoveAdapter(const std::shared_ptr<core::Object>& order) : EndCommandAdapter(order) {
}

std::string_view EndMoveAdapter::GetAdapterName() const {
  return kEndMoveAdapterName;
}

std::string_view EndMoveAdapter::GetParentAdapterName() const {
  return kEndMoveCommandName;
}

InjectableCommand* EndMoveAdapter::GetMoveCommand() {
  auto& any_move_command = GetAnyValue(kMoveCommandName);

  return CastAnyPointerToPointer<InjectableCommand>(kMoveCommandName, any_move_command);
}

std::weak_ptr<core::Object>& EndMoveAdapter::GetMovable() {
  auto& any_movable = GetAnyValue(kMovableName);

  return CastAnyRefToRef<std::weak_ptr<core::Object>>(kMovableName, any_movable);
}

Queue<std::unique_ptr<Command>>& EndMoveAdapter::GetCommandQueue() {
  auto& any_command_queue = GetAnyValue(kCommandQueueName);

  return CastAnyPointerToRef<Queue<std::unique_ptr<Command>>>(kCommandQueueName, any_command_queue);
}

void EndMoveAdapter::DoEndAction() {
  std::shared_ptr<core::Object> movable = GetMovable().lock();
  if (!movable)
    throw std::runtime_error(std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                         kEndMoveAdapterName, kEndMoveCommandName));

  auto& command_queue = GetCommandQueue();
  const auto& command = GetMoveCommand();
  movable->RemoveKey(kVelocityName);
  command->Inject(std::make_unique<NopCommand>());
}

} // namespace server::commands
