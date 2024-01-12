#include "start_rotate.h"

#include <format>

#include "exception.h"
#include "rotate.h"
#include "set_angle.h"

namespace server::commands {

StartRotateAdapter::StartRotateAdapter(const std::shared_ptr<core::Object>& order) : StartCommandAdapter(order) {
}

std::string_view StartRotateAdapter::GetAdapterName() const {
  return kStartRotateAdapterName;
}

std::string_view StartRotateAdapter::GetParentAdapterName() const {
  return kStartRotateCommandName;
}

std::weak_ptr<core::Object>& StartRotateAdapter::GetRotatable() {
  auto& any_rotatable = GetAnyValue(kRotatableName);

  return CastAnyRefToRef<std::weak_ptr<core::Object>>(kRotatableName, any_rotatable);
}

core::Angle StartRotateAdapter::GetAngle() const {
  const auto& any_angle = GetAnyValue(kAngleName);

  return CastAnyRefToValue<core::Angle>(kAngleName, any_angle);
}

Queue<std::unique_ptr<Command>>& StartRotateAdapter::GetCommandQueue() {
  auto& any_command_queue = GetAnyValue(kCommandQueueName);

  return CastAnyPointerToRef<Queue<std::unique_ptr<Command>>>(kCommandQueueName, any_command_queue);
}

void StartRotateAdapter::DoStartAction() {

  std::shared_ptr<core::Object> rotatable = GetRotatable().lock();
  if (!rotatable)
    throw sw::Exception(std::format("'{}' property value is empty for '{}' object in '{}'.", kRotatableName,
                                    kStartRotateAdapterName, kStartRotateCommandName));

  auto& command_queue = GetCommandQueue();
  const auto angle = GetAngle();

  command_queue.emplace_back(std::make_unique<SetAngle>(rotatable, angle));
  command_queue.emplace_back(std::make_unique<InjectableCommand>(std::make_unique<Rotate>(rotatable)));
}

} // namespace server::commands
