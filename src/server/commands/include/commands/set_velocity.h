#pragma once

#include <memory>
#include <string_view>

#include <cstdint>

#include "command.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kVelocityName = "Velocity";
static std::string_view kSetVelocityAdapterName = "Set velocity adapter";
static std::string_view kSetVelocityCommandName = "Set velocity command";

struct VelocitySetable {
  virtual ~VelocitySetable() = default;

  virtual void SetVelocity(core::Vector value) = 0;
};

class VelocitySetableAdapter : public VelocitySetable, public Adapter {
public:
  explicit VelocitySetableAdapter(const std::shared_ptr<core::Object>& velocity_setable);

  void SetVelocity(core::Vector value) override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class SetVelocity : public Command {
public:
  SetVelocity(const std::shared_ptr<core::Object>& velocity_setable, core::Vector velocity);
  SetVelocity(std::shared_ptr<VelocitySetable> velocity_setable, core::Vector velocity);

  void Execute() override;

private:
  std::shared_ptr<VelocitySetable> velocity_setable_;
  core::Vector velocity_;
};

} // namespace server::commands
