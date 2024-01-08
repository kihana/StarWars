#include "rotate.h"

#include <cmath>
#include <numbers>

#include "set_angle.h"
#include "move.h"

namespace server::commands {

RotatableAdapter::RotatableAdapter(const std::shared_ptr<core::Object>& rotatable) : Adapter(rotatable) {
}

std::string_view RotatableAdapter::GetAdapterName() const {
  return kRotatableName;
}

std::string_view RotatableAdapter::GetParentAdapterName() const {
  return kRotateCommandName;
}

core::Vector RotatableAdapter::GetPosition() const {
  const auto& any_position = GetAnyValue(kPositionName);

  return CastAnyRefToValue<core::Vector>(kPositionName, any_position);
}

void RotatableAdapter::SetPosition(const core::Vector value) {
  SetValue(kPositionName, value);
}

core::Angle RotatableAdapter::GetAngle() const {
  const auto& any_angle = GetAnyValue(kAngleName);

  return CastAnyRefToValue<core::Angle>(kAngleName, any_angle);
}

Rotate::Rotate(const std::shared_ptr<core::Object>& rotatable)
    : rotatable_(std::make_unique<RotatableAdapter>(rotatable)) {
}

Rotate::Rotate(std::unique_ptr<Rotatable> rotatable) : rotatable_(std::move(rotatable)) {
}

void Rotate::Execute() {
  if (!rotatable_)
    throw std::runtime_error(std::format("'{}' is unavailable.", kRotatableName));

  const auto pos = rotatable_->GetPosition();
  const auto angle = rotatable_->GetAngle() * std::numbers::pi / 180;
  rotatable_->SetPosition(core::Vector{std::llround(pos.x * std::cos(angle) - pos.y * std::sin(angle)),
                                       std::llround(pos.x * std::sin(angle) + pos.y * std::cos(angle))});
}

} // namespace server::commands
