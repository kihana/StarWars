#pragma once

#include <cstddef>

#include "command.h"
#include "core/object.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kFuelCheckableName = "FuelCheckable";
static std::string_view kCheckFuelCommandName = "CheckFuel command";

struct FuelCheckable {
  virtual ~FuelCheckable() = default;

  virtual double GetFuelLevel() const = 0;
  virtual void SetRequiredFulelLevel(double value) = 0;
  virtual core::Vector GetPosition() const = 0;
  virtual core::Vector GetVelocity() const = 0;
  virtual double GetBurnFuelVelocity() const = 0;
};

class CheckFuelAdapter : public FuelCheckable, public Adapter {
public:
  explicit CheckFuelAdapter(const std::shared_ptr<core::Object>& fuel_checkable);

  double GetFuelLevel() const override;
  void SetRequiredFulelLevel(double value) override;
  core::Vector GetPosition() const override;
  core::Vector GetVelocity() const override;
  double GetBurnFuelVelocity() const override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class CheckFuel : public Command {
public:
  explicit CheckFuel(std::shared_ptr<FuelCheckable> adapter);

  void Execute() override;

private:
  std::shared_ptr<FuelCheckable> adapter_;
};

} // namespace server::commands
