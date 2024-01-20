#include "set_velocity.h"

#include <format>

#include "core/object.h"
#include "exception.h"

namespace server::commands {

VelocitySetableAdapter::VelocitySetableAdapter(const std::shared_ptr<core::Object>& velocity_setable)
    : Adapter(velocity_setable) {
}

std::string_view VelocitySetableAdapter::GetAdapterName() const {
  return kSetVelocityAdapterName;
}

std::string_view VelocitySetableAdapter::GetParentAdapterName() const {
  return kSetVelocityCommandName;
}

void VelocitySetableAdapter::SetVelocity(const core::Vector value) {
  SetValue(kVelocityName, value);
}

SetVelocity::SetVelocity(const std::shared_ptr<core::Object>& velocity_setable, const core::Vector velocity)
    : velocity_setable_(std::make_unique<VelocitySetableAdapter>(velocity_setable)), velocity_(velocity) {
}

SetVelocity::SetVelocity(std::shared_ptr<VelocitySetable> velocity_setable, const core::Vector velocity)
    : velocity_setable_(std::move(velocity_setable)), velocity_(velocity) {
}

void SetVelocity::Execute() {
  if (!velocity_setable_)
    throw sw::Exception(std::format("'{}' is unavailable.", kSetVelocityAdapterName));

  velocity_setable_->SetVelocity(velocity_);
}

} // namespace server::commands
