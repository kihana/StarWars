#include <gtest/gtest.h>

#include "commands/end_move.h"
#include "commands/move.h"
#include "commands/set_velocity.h"
#include "commands/start_move.h"
#include "core/object.h"

namespace server::commands {

std::unique_ptr<StartCommandAdapter> MakeStartMoveAdapter(std::weak_ptr<core::Object> movable,
                                                          const core::Vector velocity,
                                                          Queue<std::unique_ptr<Command>>& command_queue);

std::shared_ptr<core::Object> MakeEndMoveOrder(std::weak_ptr<core::Object> movable,
                                               std::unique_ptr<Command>& move_command,
                                               Queue<std::unique_ptr<Command>>& command_queue) {
  auto order = std::make_shared<core::Object>();
  order->SetValue(kMovableName, std::move(movable));
  order->SetValue(kMoveCommandName, &move_command);
  order->SetValue(kCommandQueueName, &command_queue);

  return order;
}

class EndMoveTest : public testing::Test {
protected:
  void SetUp() override {
    movable_ = std::make_shared<core::Object>();
    MovableAdapter movable_adapter(movable_);
    movable_adapter.SetPosition({13, 12});
    auto start_move_adapter = MakeStartMoveAdapter(movable_, {12, 5}, command_queue_);
    StartCommand start_move_command(std::move(start_move_adapter));
    start_move_command.Execute();
  }

  std::shared_ptr<core::Object> movable_;
  Queue<std::unique_ptr<Command>> command_queue_;
};

TEST_F(EndMoveTest, Common) {
  command_queue_.front()->Execute();
  auto& move_command = command_queue_.back();
  ASSERT_EQ(command_queue_.size(), 2);
  ASSERT_TRUE(move_command);
  ASSERT_NE(nullptr, dynamic_cast<Move*>(move_command.get()));

  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  EndMove end_move(end_move_order);
  end_move.Execute();

  EXPECT_EQ(command_queue_.size(), 1);
  auto& last_command = command_queue_.back();
  EXPECT_TRUE(last_command);
  EXPECT_EQ(nullptr, dynamic_cast<Move*>(last_command.get()));
}

TEST_F(EndMoveTest, EmptyOrder) {
  std::unique_ptr<MoveEndable> empty_order;
  EndMove end_move(std::move(empty_order));
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' is unavailable.", kEndMoveAdapterName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<MoveEndable> order = std::make_unique<MoveEndableAdapter>(empty_property_holder);
  EndMove end_move(std::move(order));
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' to get '{}' is unavailable in '{}'.", kEndMoveAdapterName,
                                              kMovableName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, NoMovableInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->RemoveKey(kMovableName);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.", kMovableName,
                                              kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyAnyMovableValueInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kMovableName, std::any());
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kMovableName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, WrongMovableTypeInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kMovableName, 5);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kMovableName,
                                              kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyMovableValueInOrder) {
  std::shared_ptr<core::Object> movable;
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable);
  auto end_move_order = MakeEndMoveOrder(movable, move_command, command_queue_);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                              kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, NoCommandQueueInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->RemoveKey(kCommandQueueName);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.",
                                              kCommandQueueName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyAnyCommandQueueValueInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kCommandQueueName, std::any());
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kCommandQueueName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, WrongCommandQueueTypeInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kCommandQueueName, 5);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.",
                                              kCommandQueueName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, NoMoveCommandInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->RemoveKey(kMoveCommandName);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property is not specified for '{}' object in '{}'.",
                                              kMoveCommandName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyAnyMoveCommandValueInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kMoveCommandName, std::any());
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                              kMoveCommandName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, WrongMoveCommandTypeInOrder) {
  std::unique_ptr<Command> move_command = std::make_unique<Move>(movable_);
  auto end_move_order = MakeEndMoveOrder(movable_, move_command, command_queue_);
  end_move_order->SetValue(kMoveCommandName, 5);
  EndMove end_move(end_move_order);
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_TRUE(e.what() == std::format("Unexpected type of '{}' property in '{}' object in '{}'.",
                                              kMoveCommandName, kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

} // namespace server::commands
