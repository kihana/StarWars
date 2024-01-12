#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define EXPECT_ERROR(command, TException, message)                                                                     \
  EXPECT_THROW(                                                                                                        \
      {                                                                                                                \
        try {                                                                                                          \
          (command).Execute();                                                                                         \
        } catch (const TException& e) {                                                                                \
          EXPECT_EQ(e.what(), message);                                                                                \
          throw;                                                                                                       \
        }                                                                                                              \
      },                                                                                                               \
      TException);

namespace sw {

static std::vector<std::string> GetLogContent(const std::filesystem::path& log_path) {
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

} // namespace sw
