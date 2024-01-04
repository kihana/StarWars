#include <gtest/gtest.h>

#include "commands/set_velocity.h"

namespace server::commands {

TEST(SetVelocityTest, Common) {
  auto velocity_setable = std::make_shared<core::Object>();
  SetVelocity set_velocity_command(velocity_setable, {12, 5});
  set_velocity_command.Execute();
  auto& any_velocity = velocity_setable->GetValue(kVelocityName);
  const auto velocity = std::any_cast<core::Vector>(any_velocity);
  const core::Vector ref_velocity(12, 5);
  EXPECT_TRUE(velocity == ref_velocity);
}

TEST(SetVelocityTest, EmptyVelocitySetable) {
  std::unique_ptr<VelocitySetable> empty_velocity_setable;
  SetVelocity set_velocity(std::move(empty_velocity_setable), {12, 5});
  EXPECT_THROW(
      {
        try {
          set_velocity.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' is unavailable.", kSetVelocityAdapterName));
          throw;
        }
      },
      std::runtime_error);
}

TEST(SetVelocityTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  SetVelocity set_velocity(empty_property_holder, {12, 5});
  EXPECT_THROW(
      {
        try {
          set_velocity.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' to set '{}' is unavailable in '{}'.", kSetVelocityAdapterName,
                                          kVelocityName, kSetVelocityCommandName));
          throw;
        }
      },
      std::runtime_error);
}

} // namespace server::commands
