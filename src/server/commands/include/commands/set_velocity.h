#pragma once

#include <memory>
#include <string_view>

#include <cstdint>

#include "command.h"
#include "core/vector.h"

namespace server::core {
class Object;
}

namespace server::commands {

static std::string_view kVelocityName = "Velocity";

struct VelocitySetable {
  virtual ~VelocitySetable() = default;

  virtual void SetVelocity(core::Vector value) = 0;
};

class VelocitySetableAdapter : public VelocitySetable {
public:
  explicit VelocitySetableAdapter(const std::shared_ptr<core::Object>& velocity_setable);

  void SetVelocity(core::Vector value) override;

private:
  std::weak_ptr<core::Object> velocity_setable_;
};

class SetVelocity : public Command {
public:
  SetVelocity(const std::shared_ptr<core::Object>& velocity_setable, core::Vector velocity);

  void Execute() override;

private:
  std::unique_ptr<VelocitySetable> velocity_setable_;
  core::Vector velocity_;
};

} // namespace server::commands
