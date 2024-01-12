#include "set_angle.h"

#include <format>

#include "core/object.h"
#include "exception.h"

namespace server::commands {

SetAngleAdapter::SetAngleAdapter(const std::shared_ptr<core::Object>& angle_setable) : Adapter(angle_setable) {
}

std::string_view SetAngleAdapter::GetAdapterName() const {
  return kSetAngleAdapterName;
}

std::string_view SetAngleAdapter::GetParentAdapterName() const {
  return kSetAngleCommandName;
}

void SetAngleAdapter::SetAngle(const core::Angle value) {
  SetValue(kAngleName, value);
}

SetAngle::SetAngle(const std::shared_ptr<core::Object>& angle_setable, const core::Angle angle)
    : angle_setable_(std::make_unique<SetAngleAdapter>(angle_setable)), angle_(angle) {
}

SetAngle::SetAngle(std::unique_ptr<AngleSetable> angle_setable, const core::Angle angle)
    : angle_setable_(std::move(angle_setable)), angle_(angle) {
}

void SetAngle::Execute() {
  if (!angle_setable_)
    throw sw::Exception(std::format("'{}' is unavailable.", kSetAngleAdapterName));

  angle_setable_->SetAngle(angle_);
}

} // namespace server::commands
