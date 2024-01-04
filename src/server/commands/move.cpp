#include "move.h"

#include <format>
#include <stdexcept>

#include "commands/set_velocity.h"

namespace server::commands {

MovableAdapter::MovableAdapter(const std::shared_ptr<core::Object>& movable) : Adapter(movable) {
}

std::string_view MovableAdapter::GetAdapterName() const {
  return kMovableName;
}

std::string_view MovableAdapter::GetParentAdapterName() const {
  return kMoveCommandName;
}

core::Vector MovableAdapter::GetPosition() const {
  const auto& any_position = GetAnyValue(kPositionName);

  return CastAnyRefToValue<core::Vector>(kPositionName, any_position);
}

void MovableAdapter::SetPosition(const core::Vector value) {
  SetValue(kPositionName, value);
}

core::Vector MovableAdapter::GetVelocity() const {
  const auto& any_velocity = GetAnyValue(kVelocityName);

  return CastAnyRefToValue<core::Vector>(kVelocityName, any_velocity);
}

Move::Move(const std::shared_ptr<core::Object>& movable) : movable_(std::make_unique<MovableAdapter>(movable)) {
}

Move::Move(std::unique_ptr<Movable> movable) : movable_(std::move(movable)) {
}

void Move::Execute() {
  if (!movable_)
    throw std::runtime_error(std::format("'{}' is unavailable.", kMovableName));

  movable_->SetPosition(movable_->GetPosition() + movable_->GetVelocity());
}

} // namespace server::commands
