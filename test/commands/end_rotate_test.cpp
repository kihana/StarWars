#include <gtest/gtest.h>

#include <format>

#include "commands/end_rotate.h"
#include "commands/rotate.h"
#include "commands/set_angle.h"
#include "commands/start_rotate.h"
#include "core/object.h"
#include "exception.h"
#include "utils.h"

namespace server::commands {

extern std::unique_ptr<StartCommandAdapter> MakeStartRotateAdapter(std::weak_ptr<core::Object> rotatable,
                                                                   const core::Angle angle,
                                                                   Queue<std::unique_ptr<Command>>& command_queue);

std::shared_ptr<core::Object> MakeEndRotatePropertyHolder(std::weak_ptr<core::Object> rotatable,
                                                          std::unique_ptr<Command>& rotate_command,
                                                          Queue<std::unique_ptr<Command>>& command_queue) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kRotatableName, std::move(rotatable));
  property_holder->SetValue(kRotateCommandName, dynamic_cast<InjectableCommand*>(rotate_command.get()));
  property_holder->SetValue(kCommandQueueName, &command_queue);

  return property_holder;
}

std::unique_ptr<EndRotateAdapter> MakeEndRotateAdapter(std::weak_ptr<core::Object> rotatable,
                                                       std::unique_ptr<Command>& rotate_command,
                                                       Queue<std::unique_ptr<Command>>& command_queue) {
  return std::make_unique<EndRotateAdapter>(
      MakeEndRotatePropertyHolder(std::move(rotatable), rotate_command, command_queue));
}

class EndRotateTest : public testing::Test {
protected:
  void SetUp() override {
    rotatable_ = std::make_shared<core::Object>();
    RotatableAdapter rotatable_adapter(rotatable_);
    rotatable_adapter.SetPosition({13, 12});
    auto start_rotate_adapter = MakeStartRotateAdapter(rotatable_, 90, command_queue_);
    StartCommand start_rotate_command(std::move(start_rotate_adapter));
    start_rotate_command.Execute();
    command_queue_.front()->Execute();
    auto& rotate_command = command_queue_.back();

    end_rotate_property_holder_ = MakeEndRotatePropertyHolder(rotatable_, rotate_command, command_queue_);
    end_rotate_adapter_ = std::make_unique<EndRotateAdapter>(end_rotate_property_holder_);
  }

  std::shared_ptr<core::Object> rotatable_;
  Queue<std::unique_ptr<Command>> command_queue_;
  std::shared_ptr<core::Object> end_rotate_property_holder_;
  std::unique_ptr<EndCommandAdapter> end_rotate_adapter_;
};

TEST_F(EndRotateTest, Common) {
  ASSERT_EQ(command_queue_.size(), 2);

  EndCommand end_rotate(std::move(end_rotate_adapter_));
  end_rotate.Execute();

  EXPECT_EQ(command_queue_.size(), 2);
  auto& last_command = command_queue_.back();
  EXPECT_TRUE(last_command);
  EXPECT_NE(nullptr, dynamic_cast<InjectableCommand*>(last_command.get()));
  ASSERT_NE(nullptr,
            dynamic_cast<NopCommand*>((dynamic_cast<InjectableCommand*>(last_command.get()))->GetCommand().get()));
}

TEST_F(EndRotateTest, EmptyAdapter) {
  std::unique_ptr<EndCommandAdapter> empty_adapter;
  EndCommand end_rotate(std::move(empty_adapter));
  EXPECT_ERROR(end_rotate, sw::Exception, std::format("'{}' has not been initialized.", kEndCommandName));
}

TEST_F(EndRotateTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<EndCommandAdapter> adapter = std::make_unique<EndRotateAdapter>(empty_property_holder);
  EndCommand end_rotate(std::move(adapter));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' to get '{}' is unavailable in '{}'.", kEndRotateAdapterName, kRotatableName,
                           kEndRotateCommandName));
}

TEST_F(EndRotateTest, NoRotatableInAdapter) {
  end_rotate_property_holder_->RemoveKey(kRotatableName);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kRotatableName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, EmptyAnyRotatableValueInAdapter) {
  end_rotate_property_holder_->SetValue(kRotatableName, std::any());
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kRotatableName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, WrongRotatableTypeInAdapter) {
  end_rotate_property_holder_->SetValue(kRotatableName, 5);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kRotatableName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, EmptyRotatableValueInAdapter) {
  std::weak_ptr<core::Object> rotatable;
  end_rotate_property_holder_->SetValue(kRotatableName, std::move(rotatable));
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property value is empty for '{}' object in '{}'.", kRotatableName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, NoCommandQueueInAdapter) {
  end_rotate_property_holder_->RemoveKey(kCommandQueueName);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kCommandQueueName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, EmptyAnyCommandQueueValueInAdapter) {
  end_rotate_property_holder_->SetValue(kCommandQueueName, std::any());
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kCommandQueueName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, WrongCommandQueueTypeInAdapter) {
  end_rotate_property_holder_->SetValue(kCommandQueueName, 5);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kCommandQueueName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, NoRotateCommandInAdapter) {
  end_rotate_property_holder_->RemoveKey(kRotateCommandName);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kRotateCommandName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, EmptyAnyRotateCommandValueInAdapter) {
  end_rotate_property_holder_->SetValue(kRotateCommandName, std::any());
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kRotateCommandName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

TEST_F(EndRotateTest, WrongRotateCommandTypeInAdapter) {
  end_rotate_property_holder_->SetValue(kRotateCommandName, 5);
  EndCommand end_rotate(std::move(end_rotate_adapter_));
  EXPECT_ERROR(end_rotate, sw::Exception,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kRotateCommandName,
                           kEndRotateAdapterName, kEndRotateCommandName));
}

} // namespace server::commands
