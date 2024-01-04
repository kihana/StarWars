#include "end_move.h"

#include <format>
#include <stdexcept>

#include "move.h"
#include "set_velocity.h"

namespace server::commands {

MoveEndableAdapter::MoveEndableAdapter(const std::shared_ptr<core::Object>& order) : Adapter(order) {
}

std::string_view MoveEndableAdapter::GetAdapterName() const {
  return kEndMoveAdapterName;
}

std::string_view MoveEndableAdapter::GetParentAdapterName() const {
  return kEndMoveCommandName;
}

std::unique_ptr<Command>& MoveEndableAdapter::GetMoveCommand() {
  auto& any_move_command = GetAnyValue(kMoveCommandName);

  return CastAnyPointerToRef<std::unique_ptr<Command>>(kMoveCommandName, any_move_command);
}

std::weak_ptr<core::Object>& MoveEndableAdapter::GetMovable() {
  auto& any_movable = GetAnyValue(kMovableName);

  return CastAnyRefToRef<std::weak_ptr<core::Object>>(kMovableName, any_movable);
}

Queue<std::unique_ptr<Command>>& MoveEndableAdapter::GetCommandQueue() {
  auto& any_command_queue = GetAnyValue(kCommandQueueName);

  return CastAnyPointerToRef<Queue<std::unique_ptr<Command>>>(kCommandQueueName, any_command_queue);
}

EndMove::EndMove(const std::shared_ptr<core::Object>& order) : order_(std::make_unique<MoveEndableAdapter>(order)) {
}

EndMove::EndMove(std::unique_ptr<MoveEndable> order) : order_(std::move(order)) {
}

void EndMove::Execute() {
  if (!order_)
    throw std::runtime_error(std::format("'{}' is unavailable.", kEndMoveAdapterName));

  std::shared_ptr<core::Object> movable = order_->GetMovable().lock();
  if (!movable)
    throw std::runtime_error(std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                         kEndMoveAdapterName, kEndMoveCommandName));

  auto& command_queue = order_->GetCommandQueue();
  const auto& command = order_->GetMoveCommand();
  movable->RemoveKey(kVelocityName);
  std::erase(command_queue, command);
}

} // namespace server::commands
