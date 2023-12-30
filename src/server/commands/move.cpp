#include "move.h"

#include <stdexcept>

#include "commands/set_velocity.h"

namespace server::commands {

MovableAdapter::MovableAdapter(const std::shared_ptr<core::Object>& movable) : movable_(movable) {
}

core::Vector MovableAdapter::GetPosition() const {
  auto movable = movable_.lock();
  if (!movable)
    throw std::runtime_error("Moving object to get position is unavailable.");

  auto& any_position = movable->GetValue(kPositionName);
  if (!any_position.has_value())
    throw std::runtime_error("Position property is not specified for moving object.");

  try {
    return std::any_cast<core::Vector>(any_position);
  } catch (const std::bad_any_cast&) {
    throw std::runtime_error("Unexpected type of position property in moving object.");
  }
}

void MovableAdapter::SetPosition(const core::Vector value) {
  auto movable = movable_.lock();
  if (!movable)
    throw std::runtime_error("Moving object to set position is unavailable.");

  movable->SetValue(kPositionName, std::make_any<core::Vector>(value));
}

core::Vector MovableAdapter::GetVelocity() const {
  auto movable = movable_.lock();
  if (!movable)
    throw std::runtime_error("Moving object to get velocity is unavailable.");

  auto& any_velocity = movable->GetValue(kVelocityName);
  if (!any_velocity.has_value())
    throw std::runtime_error("Velocity property is not specified for moving object.");

  try {
    return std::any_cast<core::Vector>(any_velocity);
  } catch (const std::bad_any_cast&) {
    throw std::runtime_error("Unexpected type of velocity property in moving object.");
  }
}

Move::Move(const std::shared_ptr<core::Object>& movable) : movable_(std::make_unique<MovableAdapter>(movable)) {
}

void Move::Execute() {
  movable_->SetPosition(movable_->GetPosition() + movable_->GetVelocity());
}

} // namespace server::commands
