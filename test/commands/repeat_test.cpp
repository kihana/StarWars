#include <gtest/gtest.h>

#include <memory>

#include "commands/move.h"
#include "commands/repeat.h"
#include "commands/exception_handler.h"
#include "utils.h"

namespace server::commands {

static Queue<std::shared_ptr<Command>> g_command_queue;

std::shared_ptr<Repeatable> MakeRepeatAdapter(const std::shared_ptr<Command>& command) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kCommandName, command);

  return std::make_shared<RepeatAdapter>(property_holder);
}

void MoveExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command);
void RepeatExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command);

class RepeatTest : public testing::Test {
public:
  inline static std::uint8_t move_counter_ = 0;
  inline static std::uint8_t repeat_counter_ = 0;

protected:
  void SetUp() override {
    move_counter_ = 0;
    command_to_repeat_ = std::make_shared<Move>(std::unique_ptr<Movable>());
    g_command_queue.push_back(command_to_repeat_);
    ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(MoveExceptionHandler);
    ExceptionHandler::Instance().AddHandler<sw::Exception, Repeat>(RepeatExceptionHandler);
  }

  void TearDown() override {
  }

  std::shared_ptr<Command> command_to_repeat_;
};

void MoveExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));

  const auto adapter = MakeRepeatAdapter(command);
  g_command_queue.push_back(std::make_shared<Repeat>(adapter));

  ++RepeatTest::move_counter_;
};

void RepeatExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));

  g_command_queue.push_back(command);

  ++RepeatTest::repeat_counter_;
};

TEST_F(RepeatTest, Common) {
  const auto command = std::move(g_command_queue.front());
  g_command_queue.pop_front();

  try {
    command->Execute();
  } catch (const std::exception&) {
    const auto adapter = MakeRepeatAdapter(command);
    auto repeat_command = std::make_shared<Repeat>(adapter);
    EXPECT_ERROR(*repeat_command, sw::Exception, std::format("'{}' is unavailable.", kMovableName));
  }
}

TEST_F(RepeatTest, AddRepeatToCommandQueue) {
  while (!g_command_queue.empty()) {
    const auto command = std::move(g_command_queue.front());
    g_command_queue.pop_front();

    try {
      command->Execute();
    } catch (const std::exception& e) {
      ExceptionHandler::Instance().Handle(e, command);
    }

    if (2 == RepeatTest::repeat_counter_)
      break;
  }

  EXPECT_EQ(RepeatTest::move_counter_, 1);
  EXPECT_EQ(RepeatTest::repeat_counter_, 2);
}

} // namespace server::commands
