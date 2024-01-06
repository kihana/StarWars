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

std::shared_ptr<core::Object> MakeEndMovePropertyHolder(std::weak_ptr<core::Object> movable,
                                                        std::unique_ptr<Command>& move_command,
                                                        Queue<std::unique_ptr<Command>>& command_queue) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kMovableName, std::move(movable));
  property_holder->SetValue(kMoveCommandName, &move_command);
  property_holder->SetValue(kCommandQueueName, &command_queue);

  return property_holder;
}

std::unique_ptr<EndMoveAdapter> MakeEndMoveAdapter(std::weak_ptr<core::Object> movable,
                                                   std::unique_ptr<Command>& move_command,
                                                   Queue<std::unique_ptr<Command>>& command_queue) {
  return std::make_unique<EndMoveAdapter>(MakeEndMovePropertyHolder(std::move(movable), move_command, command_queue));
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
    command_queue_.front()->Execute();
    auto& move_command = command_queue_.back();

    end_move_property_holder_ = MakeEndMovePropertyHolder(movable_, move_command, command_queue_);
    end_move_adapter_ = std::make_unique<EndMoveAdapter>(end_move_property_holder_);
  }

  std::shared_ptr<core::Object> movable_;
  Queue<std::unique_ptr<Command>> command_queue_;
  std::shared_ptr<core::Object> end_move_property_holder_;
  std::unique_ptr<EndCommandAdapter> end_move_adapter_;
};

TEST_F(EndMoveTest, Common) {
  ASSERT_EQ(command_queue_.size(), 2);

  EndCommand end_move(std::move(end_move_adapter_));
  end_move.Execute();

  EXPECT_EQ(command_queue_.size(), 1);
  auto& last_command = command_queue_.back();
  EXPECT_TRUE(last_command);
  EXPECT_EQ(nullptr, dynamic_cast<Move*>(last_command.get()));
}

TEST_F(EndMoveTest, EmptyAdapter) {
  std::unique_ptr<EndCommandAdapter> empty_adapter;
  EndCommand end_move(std::move(empty_adapter));
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' has not been initialized.", kEndCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<EndCommandAdapter> adapter = std::make_unique<EndMoveAdapter>(empty_property_holder);
  EndCommand end_move(std::move(adapter));
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

TEST_F(EndMoveTest, NoMovableInAdapter) {
  end_move_property_holder_->RemoveKey(kMovableName);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, EmptyAnyMovableValueInAdapter) {
  end_move_property_holder_->SetValue(kMovableName, std::any());
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' property value is not specified for '{}' object in '{}'.", kMovableName,
                                          kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, WrongMovableTypeInAdapter) {
  end_move_property_holder_->SetValue(kMovableName, 5);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, EmptyMovableValueInAdapter) {
  std::weak_ptr<core::Object> movable;
  end_move_property_holder_->SetValue(kMovableName, std::move(movable));
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
  EXPECT_THROW(
      {
        try {
          end_move.Execute();
        } catch (const std::runtime_error& e) {
          EXPECT_EQ(e.what(), std::format("'{}' property value is empty for '{}' object in '{}'.", kMovableName,
                                          kEndMoveAdapterName, kEndMoveCommandName));
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(EndMoveTest, NoCommandQueueInAdapter) {
  end_move_property_holder_->RemoveKey(kCommandQueueName);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, EmptyAnyCommandQueueValueInAdapter) {
  end_move_property_holder_->SetValue(kCommandQueueName, std::any());
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, WrongCommandQueueTypeInAdapter) {
  end_move_property_holder_->SetValue(kCommandQueueName, 5);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, NoMoveCommandInAdapter) {
  end_move_property_holder_->RemoveKey(kMoveCommandName);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, EmptyAnyMoveCommandValueInAdapter) {
  end_move_property_holder_->SetValue(kMoveCommandName, std::any());
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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

TEST_F(EndMoveTest, WrongMoveCommandTypeInAdapter) {
  end_move_property_holder_->SetValue(kMoveCommandName, 5);
  EndCommand end_move(std::make_unique<EndMoveAdapter>(end_move_property_holder_));
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
