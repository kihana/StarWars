#include <gtest/gtest.h>

#include "commands/move.h"
#include "commands/set_velocity.h"
#include "core/object.h"

using namespace server::core;

namespace server::commands {

TEST(MoveTest, Common) {
  auto movable_object = std::make_shared<core::Object>();
  MovableAdapter movable_adtapter(movable_object);
  movable_adtapter.SetPosition({12, 5});
  VelocitySetableAdapter velocity_setable_adapter(movable_object);
  velocity_setable_adapter.SetVelocity({-7, 3});
  Move move(movable_object);
  move.Execute();
  EXPECT_TRUE(movable_adtapter.GetPosition() == Vector(5, 8));

  move.Execute();
  EXPECT_TRUE(movable_adtapter.GetPosition() == Vector(-2, 11));
}

} // namespace server::commands
