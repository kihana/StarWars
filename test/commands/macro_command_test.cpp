#include <gtest/gtest.h>

#include <memory>

#include "commands/burn_fuel.h"
#include "commands/check_fuel.h"
#include "commands/macro_command.h"
#include "commands/move.h"
#include "commands/set_velocity.h"
#include "commands/property_names.h"
#include "utils.h"

namespace server::commands {

static std::shared_ptr<core::Object> MakeMovable(const double fuel_level) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kPositionName, core::Vector(12, 5));
  property_holder->SetValue(propery_names::kBurnFuelVelocity, 0.5);
  property_holder->SetValue(propery_names::kFuelLevel, fuel_level);

  return property_holder;
}

class MacroCommandTest : public testing::Test {
protected:
  void SetUp() override {
    movable_ = MakeMovable(2.0);
    set_velocity_ = std::make_shared<SetVelocity>(movable_, core::Vector{-7, 3});
    check_fuel_adapter_ = std::make_shared<CheckFuelAdapter>(movable_);
    check_fuel_ = std::make_shared<CheckFuel>(check_fuel_adapter_);
    burn_fuel_ = std::make_shared<BurnFuel>(std::make_shared<BurnFuelAdapter>(movable_));
    move_adapter_ = std::make_shared<MovableAdapter>(movable_);
    move_ = std::make_shared<Move>(move_adapter_);
    macro_command_ =
        std::make_shared<MacroCommand>(std::vector<std::shared_ptr<Command>>{check_fuel_, move_, burn_fuel_});
  }

  std::shared_ptr<core::Object> movable_;
  std::shared_ptr<Command> set_velocity_;
  std::shared_ptr<FuelCheckable> check_fuel_adapter_;
  std::shared_ptr<Command> check_fuel_;
  std::shared_ptr<Command> burn_fuel_;
  std::shared_ptr<MovableAdapter> move_adapter_;
  std::shared_ptr<Command> move_;
  std::shared_ptr<Command> macro_command_;
};

TEST_F(MacroCommandTest, Common) {
  movable_->SetValue(propery_names::kFuelLevel, 4.0);
  set_velocity_->Execute();
  macro_command_->Execute();

  EXPECT_EQ(move_adapter_->GetPosition(), core::Vector(5, 8));
  EXPECT_NEAR(check_fuel_adapter_->GetFuelLevel(), 0.2, 0.01);
}

TEST_F(MacroCommandTest, FirstSubCommandException) {
  set_velocity_->Execute();
  EXPECT_ERROR(*macro_command_, sw::Exception,
               std::string("Insufficient fuel. Current amount: 2.0, required amount: 3.8"));
}

} // namespace server::commands
