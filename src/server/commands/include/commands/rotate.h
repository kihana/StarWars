#pragma once

#include "command.h"
#include "core/angle.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kRotatableName = "Rotatable";
static std::string_view kRotateCommandName = "Rotate command";

struct Rotatable {
  virtual ~Rotatable() = default;

  virtual core::Vector GetPosition() const = 0;
  virtual void SetPosition(core::Vector position) = 0;
  virtual core::Angle GetAngle() const = 0;
};

class RotatableAdapter : public Rotatable, public Adapter {
public:
  explicit RotatableAdapter(const std::shared_ptr<core::Object>& rotatable);

  core::Vector GetPosition() const override;
  void SetPosition(core::Vector value) override;
  core::Angle GetAngle() const override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class Rotate : public Command {
public:
  explicit Rotate(const std::shared_ptr<core::Object>& rotatable);
  explicit Rotate(std::unique_ptr<Rotatable> rotatable);

  void Execute() override;

private:
  std::unique_ptr<Rotatable> rotatable_;
};

} // namespace server::commands
