#include <gtest/gtest.h>

#include "core/object.h"

#include "commands/rotate.h"
#include "commands/set_angle.h"

namespace server::commands {

TEST(RotateTest, Common) {
  auto property_holder = std::make_shared<core::Object>();
  std::unique_ptr<Rotatable> rotatable = std::make_unique<RotatableAdapter>(property_holder);
  rotatable->SetPosition({3, 2});
  auto set_angle_adapter = std::make_unique<SetAngleAdapter>(property_holder);
  set_angle_adapter->SetAngle(90);

  Rotate rotate(property_holder);
  rotate.Execute();
  EXPECT_EQ(rotatable->GetPosition(), core::Vector(-2, 3));

  rotatable->SetPosition({12, 5});
  rotate.Execute();
  EXPECT_EQ(rotatable->GetPosition(), core::Vector(-5, 12));
}

} // namespace server::commands
