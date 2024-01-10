#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "commands/write_log_message.h"
#include "core/object.h"

namespace fs = std::filesystem;

namespace server::commands {

class WriteLogMessageTest : public testing::Test {
protected:
  void SetUp() override {
    log_path_ = "system.log";
    message_ = "Test message in log!";
    log_message_writtable_ = std::make_shared<core::Object>();
    write_log_message_adapter_ = std::make_unique<WriteLogMessageAdapter>(log_message_writtable_);
    write_log_message_adapter_->SetPath(log_path_);
    write_log_message_adapter_->SetMessage(message_);
  }

  void TearDown() override {
    if (!fs::remove(log_path_))
      throw std::runtime_error(std::format("Failed to remove '{}' log.", log_path_.generic_string()));
  }

  std::vector<std::string> GetLogContent() const {
    std::vector<std::string> content;

    std::ifstream file(log_path_);
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        content.push_back(std::move(line));
        line.clear();
      }
    }

    return content;
  }

  fs::path log_path_;
  std::string message_;
  std::shared_ptr<core::Object> log_message_writtable_;
  std::unique_ptr<LogMessageWritable> write_log_message_adapter_;
};

TEST_F(WriteLogMessageTest, Common) {
  WriteLogMessage command(std::move(write_log_message_adapter_));
  command.Execute();

  const auto log_content = GetLogContent();
  EXPECT_EQ(log_content.back(), message_);
}

} // namespace server::commands
