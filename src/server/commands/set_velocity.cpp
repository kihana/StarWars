#include "set_velocity.h"

#include <stdexcept>

#include "core/object.h"

namespace server::commands {

VelocitySetableAdapter::VelocitySetableAdapter(const std::shared_ptr<core::Object>& velocity_setable)
    : velocity_setable_(velocity_setable) {
}

void VelocitySetableAdapter::SetVelocity(const core::Vector value) {
  auto velocity_setable = velocity_setable_.lock();
  if (!velocity_setable)
    throw std::runtime_error("Object to set velocity is unavailable.");

  velocity_setable->SetValue(kVelocityName, std::make_any<core::Vector>(value));
}

SetVelocity::SetVelocity(const std::shared_ptr<core::Object>& velocity_setable, const core::Vector velocity)
    : velocity_setable_(std::make_unique<VelocitySetableAdapter>(velocity_setable)), velocity_(velocity) {
}

void SetVelocity::Execute() {
  velocity_setable_->SetVelocity(velocity_);
}

} // namespace server::commands
