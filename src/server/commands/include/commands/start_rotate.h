#pragma once

#include <memory>
#include <string_view>

#include "start_command.h"
#include "core/angle.h"

namespace server::commands {

static std::string_view kStartRotateAdapterName = "Start rotate order";
static std::string_view kStartRotateCommandName = "Start rotate command";

struct RotateStartable {
  virtual ~RotateStartable() = default;

  virtual std::weak_ptr<core::Object>& GetRotatable() = 0;
  virtual core::Angle GetAngle() const = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class StartRotateAdapter : public RotateStartable, public StartCommandAdapter {
public:
  explicit StartRotateAdapter(const std::shared_ptr<core::Object>& order);

  void DoStartAction() override;

private:
  std::weak_ptr<core::Object>& GetRotatable() override;
  core::Angle GetAngle() const override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

} // namespace server::commands
