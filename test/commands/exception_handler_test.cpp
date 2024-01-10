#include <exception>

#include <gtest/gtest.h>

#include "exception_handler.h"
#include "commands/move.h"

namespace server::commands {

void StdExceptionHandler(const std::exception& e, Move& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

void SwExceptionHandler(const sw::Exception& e, Command& c) {
  EXPECT_EQ(e.what(), std::format("'{}' is unavailable.", kMovableName));
};

class CommandExceptionHandlingTest : public testing::Test {
protected:
  void SetUp() override {
    ExceptionHandler::Instance().AddHandler<const std::exception&, Move&>(StdExceptionHandler);
    ExceptionHandler::Instance().AddHandler<const sw::Exception&, Command&>(SwExceptionHandler);
  }
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

} // namespace server::commands
