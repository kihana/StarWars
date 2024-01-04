#include "start_move.h"

#include <format>
#include <stdexcept>

#include "move.h"
#include "set_velocity.h"

namespace server::commands {

MoveStartableAdapter::MoveStartableAdapter(const std::shared_ptr<core::Object>& order) : Adapter(order) {
}

std::string_view MoveStartableAdapter::GetAdapterName() const {
  return kStartMoveAdapterName;
}

std::string_view MoveStartableAdapter::GetParentAdapterName() const {
  return kStartMoveCommandName;
}

std::weak_ptr<core::Object>& MoveStartableAdapter::GetMovable() {
  auto& any_movable = GetAnyValue(kMovableName);

  return CastAnyRefToRef<std::weak_ptr<core::Object>>(kMovableName, any_movable);
}

core::Vector MoveStartableAdapter::GetVelocity() const {
  const auto& any_velocity = GetAnyValue(kVelocityName);

  return CastAnyRefToValue<core::Vector>(kVelocityName, any_velocity);
}

Queue<std::unique_ptr<Command>>& MoveStartableAdapter::GetCommandQueue() {
  auto& any_command_queue = GetAnyValue(kCommandQueueName);

  return CastAnyPointerToRef<Queue<std::unique_ptr<Command>>>(kCommandQueueName, any_command_queue);
}

StartMove::StartMove(const std::shared_ptr<core::Object>& order)
    : order_(std::make_unique<MoveStartableAdapter>(order)) {
}

StartMove::StartMove(std::unique_ptr<MoveStartable> order) : order_(std::move(order)) {
}

void StartMove::Execute() {
  if (!order_)
    throw std::runtime_error(std::format("'{}' is unavailable.", kStartMoveAdapterName));

  std::shared_ptr<core::Object> movable = order_->GetMovable().lock();
  if (!movable)
    throw std::runtime_error(std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                         kStartMoveAdapterName, kStartMoveCommandName));

  auto& command_queue = order_->GetCommandQueue();
  const auto velocity = order_->GetVelocity();

  command_queue.emplace_back(std::make_unique<SetVelocity>(movable, velocity));
  command_queue.emplace_back(std::make_unique<Move>(movable));
}

} // namespace server::commands
