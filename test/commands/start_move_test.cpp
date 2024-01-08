#include <gtest/gtest.h>

#include "commands/move.h"
#include "commands/set_velocity.h"
#include "commands/start_move.h"
#include "core/object.h"

namespace server::commands {

std::shared_ptr<core::Object> MakeStartMovePropertyHolder(std::weak_ptr<core::Object> movable,
                                                          const core::Vector velocity,
                                                          Queue<std::unique_ptr<Command>>& command_queue) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kMovableName, std::move(movable));
  property_holder->SetValue(kVelocityName, velocity);
  property_holder->SetValue(kCommandQueueName, &command_queue);

  return property_holder;
}

std::unique_ptr<StartCommandAdapter> MakeStartMoveAdapter(std::weak_ptr<core::Object> movable,
                                                          const core::Vector velocity,
                                                          Queue<std::unique_ptr<Command>>& command_queue) {
  return std::make_unique<StartMoveAdapter>(MakeStartMovePropertyHolder(movable, velocity, command_queue));
}

class StartMoveTest : public testing::Test {
protected:
  void SetUp() override {
    movable_ = std::make_shared<core::Object>();
    MovableAdapter movable_adapter(movable_);
    movable_adapter.SetPosition({13, 12});
  }

  std::shared_ptr<core::Object> movable_;
  Queue<std::unique_ptr<Command>> command_queue_;
};

TEST_F(StartMoveTest, Common) {
  auto start_move_adapter = MakeStartMoveAdapter(movable_, {12, 5}, command_queue_);
  StartCommand start_move_command(std::move(start_move_adapter));
  start_move_command.Execute();

  auto& set_velocity_command = command_queue_.front();
  auto& move_command = command_queue_.back();
  ASSERT_EQ(command_queue_.size(), 2);
  ASSERT_TRUE(set_velocity_command);
  ASSERT_TRUE(move_command);
  ASSERT_NE(nullptr, dynamic_cast<SetVelocity*>(set_velocity_command.get()));
  ASSERT_NE(nullptr, dynamic_cast<Move*>(move_command.get()));
}

TEST_F(StartMoveTest, EmptyAdapter) {
  std::unique_ptr<StartCommandAdapter> empty_adapter;
  StartCommand start_move(std::move(empty_adapter));
  EXPECT_THROW(
      {
        try {
          start_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' has not been initialized.", kStartCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(StartMoveTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(empty_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, NoMovableInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->RemoveKey(kMovableName);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, EmptyAnyMovableValueInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kMovableName, std::any());
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, WrongMovableTypeInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kMovableName, 5);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, EmptyMovableValueInAdapter) {
  std::weak_ptr<core::Object> movable;
  std::unique_ptr<StartCommandAdapter> adapter = MakeStartMoveAdapter(std::move(movable), {12, 5}, command_queue_);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, NoCommandQueueInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->RemoveKey(kCommandQueueName);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, EmptyAnyCommandQueueValueInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kCommandQueueName, std::any());
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, WrongCommandQueueTypeInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kCommandQueueName, 5);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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

TEST_F(StartMoveTest, NoVelocityInAdapter) {
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->RemoveKey(kVelocityName);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kVelocityName, std::any());
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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
  auto start_move_property_holder = MakeStartMovePropertyHolder(movable_, {12, 5}, command_queue_);
  start_move_property_holder->SetValue(kVelocityName, 5);
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartMoveAdapter>(start_move_property_holder);
  StartCommand start_move(std::move(adapter));
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
