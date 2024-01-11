#include <exception>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "commands/exception_handler.h"
#include "commands/move.h"
#include "commands/write_log_message.h"

namespace fs = std::filesystem;

namespace server::commands {

const fs::path kLogPath = "exception_handler_test.log";

static Queue<std::shared_ptr<Command>> g_command_queue;

std::vector<std::string> GetLogContent(const fs::path& log_path) {
  std::vector<std::string> content;

  std::ifstream file(log_path);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      content.push_back(std::move(line));
      line.clear();
    }
  }

  return content;
}

std::shared_ptr<WriteLogMessageAdapter> MakeWriteLogMessageAdapter(const std::string& message) {
  auto property_holder = std::make_shared<core::Object>();
  property_holder->SetValue(kLogPathName, kLogPath);
  property_holder->SetValue(kLogMessageName, message);

  return std::make_shared<WriteLogMessageAdapter>(property_holder);
}

static void MoveExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

void MoveSwExceptionHandler(const std::exception& e, const std::shared_ptr<Command>& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

void WriteExceptionMessageHandler(const std::exception& e, const std::shared_ptr<Command>& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
  auto adapter = MakeWriteLogMessageAdapter(e.what());
  WriteLogMessage logger(adapter);
  logger.Execute();

  const auto log_content = GetLogContent(kLogPath);
  EXPECT_EQ(e.what(), log_content.back());
};

auto add_write_exception_message_to_command_queue = [](const std::exception& e, const std::shared_ptr<Command>& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));

  g_command_queue.push_back(std::make_shared<WriteLogMessage>(MakeWriteLogMessageAdapter(e.what())));
};

class CommandExceptionHandlingTest : public testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
    if (fs::exists(kLogPath) && !fs::remove(kLogPath))
      throw std::runtime_error(std::format("Failed to remove '{}' log file.", kLogPath.generic_string()));
  }

  std::unique_ptr<WriteLogMessage> write_log_message_command_;
};

TEST_F(CommandExceptionHandlingTest, CommonStdException) {
  ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(MoveExceptionHandler);

  std::unique_ptr<Movable> empty_movable;
  auto move_command = std::make_shared<Move>(std::move(empty_movable));
  try {
    move_command->Execute();
  } catch (const std::exception& e) {
    ExceptionHandler::Instance().Handle(e, move_command);
  }
}

TEST_F(CommandExceptionHandlingTest, CommonSwException) {
  ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(MoveSwExceptionHandler);

  std::unique_ptr<Movable> empty_movable;
  std::shared_ptr<Command> move_command = std::make_shared<Move>(std::move(empty_movable));
  try {
    move_command->Execute();
  } catch (const sw::Exception& e) {
    ExceptionHandler::Instance().Handle(e, move_command);
  }
}

TEST_F(CommandExceptionHandlingTest, CommonWriteException) {
  ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(WriteExceptionMessageHandler);

  std::unique_ptr<Movable> empty_movable;
  std::shared_ptr<Command> move_command = std::make_shared<Move>(std::move(empty_movable));
  try {
    move_command->Execute();
  } catch (const std::exception& e) {
    ExceptionHandler::Instance().Handle(e, move_command);
  }
}

TEST_F(CommandExceptionHandlingTest, AddWriteExceptionToCommandQueue) {
  ExceptionHandler::Instance().AddHandler<sw::Exception, Move>(add_write_exception_message_to_command_queue);

  std::unique_ptr<Movable> empty_movable;

  g_command_queue.push_back(std::make_shared<Move>(std::move(empty_movable)));

  while (!g_command_queue.empty()) {
    const auto command = std::move(g_command_queue.front());
    g_command_queue.pop_front();

    try {
      command->Execute();
    } catch (const std::exception& e) {
      ExceptionHandler::Instance().Handle(e, command);
    }
  }

  const auto log_content = GetLogContent(kLogPath);
  EXPECT_EQ(std::format("'{}' is unavailable.", kMovableName), log_content.back());
}

} // namespace server::commands
