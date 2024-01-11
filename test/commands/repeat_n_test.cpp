#include <gtest/gtest.h>

#include <memory>

#include "commands/exception_handler.h"
#include "commands/move.h"
#include "commands/repeat.h"
#include "commands/repeat_n.h"
#include "commands/write_log_message.h"
#include "utils.h"

namespace fs = std::filesystem;

namespace server::commands {

static const fs::path kLogPath = "exception_handler_test.log";

static Queue<std::shared_ptr<Command>> g_command_queue;

static std::shared_ptr<NRepeatable> MakeRepeatNAdapter(const std::shared_ptr<Command>& command,
                                                       const std::string& message) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kCommandName, command);
  property_holder->SetValue(kErrorMessageName, message);

  return std::make_shared<RepeatNAdapter>(property_holder);
}

static std::shared_ptr<WriteLogMessageAdapter> MakeWriteLogMessageAdapter(const std::string& message) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kLogPathName, kLogPath);
  property_holder->SetValue(kLogMessageName, message);

  return std::make_shared<WriteLogMessageAdapter>(property_holder);
}

void MoveExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command);
void RepeatExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command);
void WriteMessageExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command);

class RepeatNTest : public testing::Test {
public:
  inline static std::uint8_t move_counter_ = 0;
  inline static std::uint8_t repeat_counter_ = 0;

protected:
  void SetUp() override {
    move_counter_ = 0;
    command_to_repeat_ = std::make_shared<Move>(std::unique_ptr<Movable>());
    g_command_queue.push_back(command_to_repeat_);
    ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(MoveExceptionHandler);
    ExceptionHandler::Instance().AddHandler<sw::Exception, RepeatN>(RepeatExceptionHandler);
    ExceptionHandler::Instance().AddHandler<RepeatNException, RepeatN>(WriteMessageExceptionHandler);
  }

  void TearDown() override {
    if (fs::exists(kLogPath) && !fs::remove(kLogPath))
      throw std::runtime_error(std::format("Failed to remove '{}' log file.", kLogPath.generic_string()));
  }

  std::shared_ptr<Command> command_to_repeat_;
};

static void MoveExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));

  const auto adapter = MakeRepeatNAdapter(command, e.what());
  g_command_queue.push_back(std::make_shared<RepeatN>(adapter, 1));

  ++RepeatNTest::move_counter_;
};

static void RepeatExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));

  g_command_queue.push_back(command);

  ++RepeatNTest::repeat_counter_;
};

static void WriteMessageExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& command) {
  EXPECT_EQ(e.what(), std::format("Command has been executed {} times.", 2));

  const auto original_exception = dynamic_cast<const RepeatNException&>(e);
  auto adapter = MakeWriteLogMessageAdapter(original_exception.GetOriginalMessage());
  WriteLogMessage logger(adapter);
  logger.Execute();

  const auto log_content = sw::GetLogContent(kLogPath);
  EXPECT_EQ(1, log_content.size());
  EXPECT_EQ(std::format("'{}' is unavailable.", kMovableName), log_content.back());
};

TEST_F(RepeatNTest, Common) {
  const auto command = std::move(g_command_queue.front());
  g_command_queue.pop_front();

  try {
    command->Execute();
  } catch (const std::exception& e) {
    const auto adapter = MakeRepeatNAdapter(command, e.what());
    auto repeat_command = std::make_shared<RepeatN>(adapter, 1);
    EXPECT_ERROR(*repeat_command, sw::Exception, std::format("'{}' is unavailable.", kMovableName));
  }
}

TEST_F(RepeatNTest, AddRepeatToCommandQueue) {
  while (!g_command_queue.empty()) {
    const auto command = std::move(g_command_queue.front());
    g_command_queue.pop_front();

    try {
      command->Execute();
    } catch (const std::exception& e) {
      ExceptionHandler::Instance().Handle(e, command);
    }
  }

  EXPECT_EQ(RepeatNTest::move_counter_, 1);
  EXPECT_EQ(RepeatNTest::repeat_counter_, 1);

  const auto log_content = sw::GetLogContent(kLogPath);
  EXPECT_EQ(1, log_content.size());
  EXPECT_EQ(std::format("'{}' is unavailable.", kMovableName), log_content.back());
}

} // namespace server::commands
