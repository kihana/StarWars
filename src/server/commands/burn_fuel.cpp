#include "burn_fuel.h"

#include <format>

#include "property_names.h"

namespace server::commands {

BurnFuelAdapter::BurnFuelAdapter(const std::shared_ptr<core::Object>& fuel_burnable) : Adapter(fuel_burnable) {
}

double BurnFuelAdapter::GetRequiredFuelLevel() const {
  const auto& any_value = GetAnyValue(propery_names::kRequiredFuelLevel);

  return CastAnyRefToValue<double>(propery_names::kRequiredFuelLevel, any_value);
}

double BurnFuelAdapter::GetFuelLevel() const {
  const auto& any_value = GetAnyValue(propery_names::kFuelLevel);

  return CastAnyRefToValue<double>(propery_names::kFuelLevel, any_value);
}

void BurnFuelAdapter::SetFuelLevel(double value) {
  SetValue(propery_names::kFuelLevel, value);
}

std::string_view BurnFuelAdapter::GetAdapterName() const {
  return kFuelBurnableName;
}

std::string_view BurnFuelAdapter::GetParentAdapterName() const {
  return kBurnFuelCommandName;
}

BurnFuel::BurnFuel(const std::shared_ptr<FuelBurnable> adapter) : adapter_(adapter) {
}

void BurnFuel::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kFuelBurnableName));

  const auto required_fuel = adapter_->GetRequiredFuelLevel();
  const auto current_fuel = adapter_->GetFuelLevel();
  adapter_->SetFuelLevel(current_fuel - required_fuel);
}

} // namespace server::commands
