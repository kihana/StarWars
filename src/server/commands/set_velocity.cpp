#include "set_velocity.h"

#include <format>

#include "core/object.h"
#include "exception.h"

namespace server::commands {

VelocitySetableAdapter::VelocitySetableAdapter(std::shared_ptr<core::Object> velocity_setable)
    : Adapter(std::move(velocity_setable)) {
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

SetVelocity::SetVelocity(std::shared_ptr<VelocitySetable> adapter, const core::Vector velocity)
    : adapter_(std::move(adapter)), velocity_(velocity) {
}

void SetVelocity::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kSetVelocityAdapterName));

  adapter_->SetVelocity(velocity_);
}

} // namespace server::commands
