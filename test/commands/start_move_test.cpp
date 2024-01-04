#include <gtest/gtest.h>

#include "commands/start_move.h"
#include "commands/move.h"
#include "commands/set_velocity.h"
#include "commands/start_move.h"
#include "core/object.h"

namespace server::commands {

std::shared_ptr<core::Object> MakeStartMoveOrder(std::weak_ptr<core::Object> movable, const core::Vector velocity,
                                                 Queue<std::unique_ptr<Command>>& command_queue) {
  auto order = std::make_shared<core::Object>();
  order->SetValue(kMovableName, std::move(movable));
  order->SetValue(kVelocityName, velocity);
  order->SetValue(kCommandQueueName, &command_queue);

  return order;
}

class StartMoveTest : public testing::Test {
protected:
  void SetUp() override {
    movable_ = std::make_shared<core::Object>();
    MovableAdapter movable_adapter(movable_);
    movable_adapter.SetPosition({13, 12});
    start_move_order_ = MakeStartMoveOrder(movable_, {12, 5}, command_queue_);
  }

  std::shared_ptr<core::Object> movable_;
  std::shared_ptr<core::Object> start_move_order_;
  Queue<std::unique_ptr<Command>> command_queue_;
};

TEST_F(StartMoveTest, Common) {
  StartMove start_move(start_move_order_);
  start_move.Execute();

  auto& set_velocity_command = command_queue_.front();
  auto& move_command = command_queue_.back();
  ASSERT_EQ(command_queue_.size(), 2);
  ASSERT_TRUE(set_velocity_command);
  ASSERT_TRUE(move_command);
  ASSERT_NE(nullptr, dynamic_cast<SetVelocity*>(set_velocity_command.get()));
  ASSERT_NE(nullptr, dynamic_cast<Move*>(move_command.get()));
}

TEST_F(StartMoveTest, EmptyOrder) {
  std::unique_ptr<MoveStartable> empty_order;
  StartMove start_move(std::move(empty_order));
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' is unavailable.", kStartMoveAdapterName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<MoveStartable> order = std::make_unique<MoveStartableAdapter>(empty_property_holder);
  StartMove start_move(std::move(order));
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' to get '{}' is unavailable in '{}'.", kStartMoveAdapterName,
                                              kMovableName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, NoMovableInOrder) {
  start_move_order_->RemoveKey(kMovableName);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.", kMovableName,
                                              kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyAnyMovableValueInOrder) {
  start_move_order_->SetValue(kMovableName, std::any());
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kMovableName, kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, WrongMovableTypeInOrder) {
  start_move_order_->SetValue(kMovableName, 5);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kMovableName,
                                              kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyMovableValueInOrder) {
  std::weak_ptr<core::Object> movable;
  start_move_order_->SetValue(kMovableName, std::move(movable));
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                          kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, NoCommandQueueInOrder) {
  start_move_order_->RemoveKey(kCommandQueueName);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.",
                                              kCommandQueueName, kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyAnyCommandQueueValueInOrder) {
  start_move_order_->SetValue(kCommandQueueName, std::any());
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kCommandQueueName, kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, WrongCommandQueueTypeInOrder) {
  start_move_order_->SetValue(kCommandQueueName, 5);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.",
                                              kCommandQueueName, kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, NoVelocityInOrder) {
  start_move_order_->RemoveKey(kVelocityName);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.", kVelocityName,
                                              kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyAnyVelocityValueInOrder) {
  start_move_order_->SetValue(kVelocityName, std::any());
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kVelocityName, kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, WrongVelocityTypeInOrder) {
  start_move_order_->SetValue(kVelocityName, 5);
  StartMove start_move(start_move_order_);
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kVelocityName,
                                              kStartMoveAdapterName, kStartMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

} // namespace server::commands
