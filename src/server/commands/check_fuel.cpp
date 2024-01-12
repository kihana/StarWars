#include "check_fuel.h"

#include <format>

#include <cmath>

#include "move.h"
#include "property_names.h"
#include "set_velocity.h"

namespace server::commands {

CheckFuelAdapter::CheckFuelAdapter(const std::shared_ptr<core::Object>& fuel_checkable) : Adapter(fuel_checkable) {
}

double CheckFuelAdapter::GetFuelLevel() const {
  const auto& any_fuel_level = GetAnyValue(propery_names::kFuelLevel);

  return CastAnyRefToValue<double>(propery_names::kFuelLevel, any_fuel_level);
}

core::Vector CheckFuelAdapter::GetPosition() const {
  const auto& any_position = GetAnyValue(kPositionName);

  return CastAnyRefToValue<core::Vector>(kPositionName, any_position);
}

core::Vector CheckFuelAdapter::GetVelocity() const {
  const auto& any_velocity = GetAnyValue(kVelocityName);

  return CastAnyRefToValue<core::Vector>(kVelocityName, any_velocity);
}

double CheckFuelAdapter::GetBurnFuelVelocity() const {
  const auto& any_value = GetAnyValue(propery_names::kBurnFuelVelocity);

  return CastAnyRefToValue<double>(propery_names::kBurnFuelVelocity, any_value);
}

std::string_view CheckFuelAdapter::GetAdapterName() const {
  return kFuelCheckableName;
}

std::string_view CheckFuelAdapter::GetParentAdapterName() const {
  return kCheckFuelCommandName;
}

CheckFuel::CheckFuel(const std::shared_ptr<FuelCheckable> adapter) : adapter_(adapter) {
}

bool GreaterThan(const double a, const double b, const double epsilon) {
  return (a - b) > ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
}

void CheckFuel::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kFuelCheckableName));

  const auto old_position = adapter_->GetPosition();
  const auto new_position = old_position + adapter_->GetVelocity();
  const auto distance = std::hypot((new_position.x - old_position.x), (new_position.y - old_position.y));

  const auto required_fuel = adapter_->GetBurnFuelVelocity() * distance;
  const auto current_fuel = adapter_->GetFuelLevel();
  if (GreaterThan(required_fuel, current_fuel, 0.1))
    throw sw::Exception(
        std::format("Insufficient fuel. Current amount: {:.1f}, required amount: {:.1f}", current_fuel, required_fuel));
}

} // namespace server::commands
