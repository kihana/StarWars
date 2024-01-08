#pragma once

#include <memory>
#include <string_view>

#include "command.h"
#include "core/angle.h"

namespace server::commands {

static std::string_view kAngleName = "Angle";
static std::string_view kSetAngleAdapterName = "Set angle adapter";
static std::string_view kSetAngleCommandName = "Set angle command";

struct AngleSetable {
  virtual ~AngleSetable() = default;

  virtual void SetAngle(core::Angle value) = 0;
};

class SetAngleAdapter : public AngleSetable, public Adapter {
public:
  explicit SetAngleAdapter(const std::shared_ptr<core::Object>& angle_setable);

  void SetAngle(core::Angle value) override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class SetAngle : public Command {
public:
  SetAngle(const std::shared_ptr<core::Object>& velocity_setable, core::Angle angle);
  SetAngle(std::unique_ptr<AngleSetable> velocity_setable, core::Angle angle);

  void Execute() override;

private:
  std::unique_ptr<AngleSetable> angle_setable_;
  core::Angle angle_;
};

} // namespace server::commands
