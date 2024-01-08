#include <gtest/gtest.h>

#include "commands/set_angle.h"
#include "commands/start_rotate.h"
#include "commands/rotate.h"
#include "core/object.h"
#include "utils.h"

namespace server::commands {

std::shared_ptr<core::Object> MakeStartRotatePropertyHolder(std::weak_ptr<core::Object> rotatable,
                                                            const core::Angle angle,
                                                            Queue<std::unique_ptr<Command>>& command_queue) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kRotatableName, std::move(rotatable));
  property_holder->SetValue(kAngleName, angle);
  property_holder->SetValue(kCommandQueueName, &command_queue);

  return property_holder;
}

std::unique_ptr<StartCommandAdapter> MakeStartRotateAdapter(std::weak_ptr<core::Object> rotatable,
                                                            const core::Angle angle,
                                                            Queue<std::unique_ptr<Command>>& command_queue) {
  return std::make_unique<StartRotateAdapter>(MakeStartRotatePropertyHolder(rotatable, angle, command_queue));
}

class StartRotateTest : public testing::Test {
protected:
  void SetUp() override {

    rotatable_ = std::make_shared<core::Object>();
    start_rotate_property_holder_ = MakeStartRotatePropertyHolder(rotatable_, 90, command_queue_);
    start_rotate_adapter_ = std::make_unique<StartRotateAdapter>(start_rotate_property_holder_);
    RotatableAdapter adapter(rotatable_);
    adapter.SetPosition({3, 2});
  }

  std::shared_ptr<core::Object> start_rotate_property_holder_;
  std::unique_ptr<StartCommandAdapter> start_rotate_adapter_;
  std::shared_ptr<core::Object> rotatable_;
  Queue<std::unique_ptr<Command>> command_queue_;
};

TEST_F(StartRotateTest, Common) {
  auto adapter = MakeStartRotateAdapter(rotatable_, 9, command_queue_);
  StartCommand start_move_command(std::move(adapter));
  start_move_command.Execute();

  auto& set_angle_command = command_queue_.front();
  auto& rotate_command = command_queue_.back();
  ASSERT_EQ(command_queue_.size(), 2);
  ASSERT_TRUE(set_angle_command);
  ASSERT_TRUE(rotate_command);
  ASSERT_NE(nullptr, dynamic_cast<SetAngle*>(set_angle_command.get()));
  ASSERT_NE(nullptr, dynamic_cast<InjectableCommand*>(rotate_command.get()));
  ASSERT_NE(nullptr,
            dynamic_cast<Rotate*>((dynamic_cast<InjectableCommand*>(rotate_command.get()))->GetCommand().get()));
}

TEST_F(StartRotateTest, EmptyAdapter) {
  std::unique_ptr<StartCommandAdapter> empty_adapter;
  StartCommand start_rotate(std::move(empty_adapter));
  EXPECT_ERROR(start_rotate, std::runtime_error, std::format("'{}' has not been initialized.", kStartCommandName));
}

TEST_F(StartRotateTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> empty_property_holder;
  std::unique_ptr<StartCommandAdapter> adapter = std::make_unique<StartRotateAdapter>(empty_property_holder);
  StartCommand start_rotate(std::move(adapter));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' to get '{}' is unavailable in '{}'.", kStartRotateAdapterName, kRotatableName,
                           kStartRotateCommandName));
}

TEST_F(StartRotateTest, NoRotatableInAdapter) {
  start_rotate_property_holder_->RemoveKey(kRotatableName);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kRotatableName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, EmptyAnyRotatableValueInAdapter) {
  start_rotate_property_holder_->SetValue(kRotatableName, std::any());
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kRotatableName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, WrongRotatableTypeInAdapter) {
  start_rotate_property_holder_->SetValue(kRotatableName, 5);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kRotatableName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, EmptyRotatableValueInAdapter) {
  std::weak_ptr<core::Object> rotatable;
  std::unique_ptr<StartCommandAdapter> adapter = MakeStartRotateAdapter(std::move(rotatable), 90, command_queue_);
  StartCommand start_rotate(std::move(adapter));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property value is empty for '{}' object in '{}'.", kRotatableName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, NoCommandQueueInAdapter) {
  start_rotate_property_holder_->RemoveKey(kCommandQueueName);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kCommandQueueName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, EmptyAnyCommandQueueValueInAdapter) {
  start_rotate_property_holder_->SetValue(kCommandQueueName, std::any());
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kCommandQueueName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, WrongCommandQueueTypeInAdapter) {
  start_rotate_property_holder_->SetValue(kCommandQueueName, 5);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kCommandQueueName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, NoAngleInAdapter) {
  start_rotate_property_holder_->RemoveKey(kAngleName);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property is not specified for '{}' object in '{}'.", kAngleName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, EmptyAnyAngleValueInOrder) {
  start_rotate_property_holder_->SetValue(kAngleName, std::any());
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("'{}' property value is not specified for '{}' object in '{}'.", kAngleName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

TEST_F(StartRotateTest, WrongAngleTypeInOrder) {
  start_rotate_property_holder_->SetValue(kAngleName, 5);
  StartCommand start_rotate(std::move(start_rotate_adapter_));
  EXPECT_ERROR(start_rotate, std::runtime_error,
               std::format("Unexpected type of '{}' property in '{}' object in '{}'.", kAngleName,
                           kStartRotateAdapterName, kStartRotateCommandName));
}

} // namespace server::commands
