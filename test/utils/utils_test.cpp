#include <gtest/gtest.h>

#include "utils/move.h"

using namespace server::utils;
using namespace server::core;

class TestMovable : public Movable {
public:
  Vector GetPosition() const override {
    return position_;
  }
  void SetPosition(const Vector position) override {
    position_ = position;
  }
  Vector GetVelocity() const override {
    return velocity_;
  }
  void SetVelocity(const Vector velocity) override {
    velocity_ = velocity;
  }

private:
  Vector position_;
  Vector velocity_;
};

TEST(Utils, MoveTest) {
  TestMovable movable;
  movable.SetPosition({12, 5});
  movable.SetVelocity({-7, 3});
  Move move(movable);
  move.Execute();

  EXPECT_TRUE(movable.GetPosition() == Vector(5, 8));
}