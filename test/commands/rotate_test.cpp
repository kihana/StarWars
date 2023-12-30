#include <gtest/gtest.h>

#include "commands/rotate.h"

using namespace server::commands;
using namespace server::core;

class TestRotatable : public Rotatable {
public:
  Vector GetPosition() const override {
    return position_;
  }
  void SetPosition(const Vector position) override {
    position_ = position;
  }
  Angle GetAngle() const override {
    return angle_;
  }
  void SetAngle(const Angle angle) override {
    angle_ = angle;
  }

private:
  Vector position_;
  Angle angle_;
};

TEST(Utils, RotateTest) {
  TestRotatable rotatable;
  rotatable.SetPosition({3, 2});
  rotatable.SetAngle(90);
  Rotate rotate(rotatable);
  rotate.Execute();
  EXPECT_TRUE(rotatable.GetPosition() == Vector(-2, 3));

  rotatable.SetPosition({12, 5});
  rotate.Execute();
  EXPECT_TRUE(rotatable.GetPosition() == Vector(-5, 12));
}