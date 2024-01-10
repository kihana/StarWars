#pragma once

#include <filesystem>
#include <string>

#include "command.h"

namespace server::commands {

static std::string_view kLogPathName = "Log path";
static std::string_view kLogMessageName = "Log message";
static std::string_view kLogMessageWritableName = "Log message writable";
static std::string_view kWriteLogMessageCommandName = "Write log message command";

struct LogMessageWritable {
  virtual ~LogMessageWritable() = default;

  virtual const std::filesystem::path& GetPath() const = 0;
  virtual void SetPath(const std::filesystem::path& value) = 0;
  virtual const std::string& GetMessage() const = 0;
  virtual void SetMessage(const std::string& value) = 0;
};

class WriteLogMessageAdapter : public LogMessageWritable, public Adapter {
public:
  explicit WriteLogMessageAdapter(const std::shared_ptr<core::Object>& log_message_writable);

  const std::filesystem::path& GetPath() const override;
  void SetPath(const std::filesystem::path& value) override;
  const std::string& GetMessage() const override;
  void SetMessage(const std::string& value) override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class WriteLogMessage final : public Command {
public:
  explicit WriteLogMessage(const std::shared_ptr<LogMessageWritable>& adapter);

  void Execute() final;

private:
  std::shared_ptr<LogMessageWritable> adapter_;
};

} // namespace server::commands