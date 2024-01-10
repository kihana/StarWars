#include <exception>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "exception_handler.h"
#include "commands/move.h"
#include "commands/write_log_message.h"

namespace fs = std::filesystem;

namespace server::commands {

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

void StdExceptionHandler(const std::exception& e, Move& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

void SwExceptionHandler(const sw::Exception& e, Command& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

void WriteExceptionMessageHandler(const std::exception& e, Command& c,
                                  std::shared_ptr<WriteLogMessageAdapter>& adapter) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
  adapter->SetMessage(e.what());
  WriteLogMessage logger(adapter);
  logger.Execute();

  const auto log_content = GetLogContent(adapter->GetPath());
  EXPECT_EQ(e.what(), log_content.back());
};

Queue<std::unique_ptr<Command>> g_command_queue;
auto g_write_log_message_adapter = std::make_shared<WriteLogMessageAdapter>(std::make_shared<core::Object>());

auto add_write_exception_message_to_command_queue = [](const std::exception& e, const std::unique_ptr<Command>& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
  g_write_log_message_adapter->SetMessage(e.what());
  g_command_queue.push_back(std::make_unique<WriteLogMessage>(g_write_log_message_adapter));
};

class CommandExceptionHandlingTest : public testing::Test {
protected:
  void SetUp() override {
    log_path_ = "exception_handler_test.log";
    g_write_log_message_adapter->SetPath(log_path_);
    ExceptionHandler::Instance().AddHandler<const std::exception&, Move&>(StdExceptionHandler);
    ExceptionHandler::Instance().AddHandler<const sw::Exception&, Command&>(SwExceptionHandler);
    ExceptionHandler::Instance().AddHandler<const std::exception&, Command&, std::shared_ptr<WriteLogMessageAdapter>&>(
        WriteExceptionMessageHandler);
    ExceptionHandler::Instance().AddHandler<const std::exception&, const std::unique_ptr<Command>&>(
        add_write_exception_message_to_command_queue);
  }

  void TearDown() override {
    if (fs::exists(log_path_) && !fs::remove(log_path_))
      throw std::runtime_error(std::format("Failed to remove '{}' log file.", log_path_));
  }

  std::string log_path_;

  std::unique_ptr<WriteLogMessage> write_log_message_command_;
};

TEST_F(CommandExceptionHandlingTest, CommonStdException) {
  std::unique_ptr<Movable> empty_movable;
  Move move_command(std::move(empty_movable));
  try {
    move_command.Execute();
  } catch (const std::exception& e) {
    ExceptionHandler::Instance().Handle(e, move_command);
  }
}

TEST_F(CommandExceptionHandlingTest, CommonSwException) {
  std::unique_ptr<Movable> empty_movable;
  std::unique_ptr<Command> move_command = std::make_unique<Move>(std::move(empty_movable));
  try {
    move_command->Execute();
  } catch (const sw::Exception& e) {
    ExceptionHandler::Instance().Handle(e, *move_command);
  }
}

TEST_F(CommandExceptionHandlingTest, CommonWriteException) {
  std::unique_ptr<Movable> empty_movable;
  std::unique_ptr<Command> move_command = std::make_unique<Move>(std::move(empty_movable));
  try {
    move_command->Execute();
  } catch (const std::exception& e) {
    ExceptionHandler::Instance().Handle(e, *move_command, g_write_log_message_adapter);
  }
}

TEST_F(CommandExceptionHandlingTest, AddWriteExceptionToCommandQueue) {
  std::unique_ptr<Movable> empty_movable;
  std::unique_ptr<Command> move_command = std::make_unique<Move>(std::move(empty_movable));

  g_command_queue.push_back(std::move(move_command));

  while (!g_command_queue.empty()) {
    const auto command = std::move(g_command_queue.front());
    g_command_queue.pop_front();

    try {
      command->Execute();
    } catch (const std::exception& e) {
      ExceptionHandler::Instance().Handle(e, command);
    }
  }

  const auto log_content = GetLogContent(g_write_log_message_adapter->GetPath());
  EXPECT_EQ(std::format("'{}' is unavailable.", kMovableName), log_content.back());
}

} // namespace server::commands
