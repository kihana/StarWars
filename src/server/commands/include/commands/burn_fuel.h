#pragma once

#include <cstddef>

#include "command.h"
#include "core/object.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kFuelBurnableName = "FuelBurnable";
static std::string_view kBurnFuelCommandName = "BurnFuel command";

struct FuelBurnable {
  virtual ~FuelBurnable() = default;

  virtual double GetRequiredFuelLevel() const = 0;
  virtual double GetFuelLevel() const = 0;
  virtual void SetFuelLevel(double value) = 0;
};

class BurnFuelAdapter : public FuelBurnable, public Adapter {
public:
  explicit BurnFuelAdapter(const std::shared_ptr<core::Object>& fuel_burnable);

  double GetRequiredFuelLevel() const override;
  double GetFuelLevel() const override;
  void SetFuelLevel(double value) override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class BurnFuel : public Command {
public:
  explicit BurnFuel(std::shared_ptr<FuelBurnable> adapter);

  void Execute() override;

private:
  std::shared_ptr<FuelBurnable> adapter_;
};

} // namespace server::commands
