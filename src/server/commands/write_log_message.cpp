#include "write_log_message.h"

#include <format>
#include <fstream>

namespace fs = std::filesystem;

namespace server::commands {

WriteLogMessageAdapter::WriteLogMessageAdapter(const std::shared_ptr<core::Object>& log_message_writable)
    : Adapter(log_message_writable) {
}

const fs::path& WriteLogMessageAdapter::GetPath() const {
  const auto& any_path = GetAnyValue(kLogPathName);

  return CastAnyRefToRef<const fs::path>(kLogPathName, any_path);
}
void WriteLogMessageAdapter::SetPath(const fs::path& value) {
  SetValue(kLogPathName, value);
}

const std::string& WriteLogMessageAdapter::GetMessage() const {
  const auto& any_message = GetAnyValue(kLogMessageName);

  return CastAnyRefToRef<const std::string>(kLogMessageName, any_message);
}

void WriteLogMessageAdapter::SetMessage(const std::string& value) {
  SetValue(kLogMessageName, value);
}

std::string_view WriteLogMessageAdapter::GetAdapterName() const {
  return kLogMessageWritableName;
}

std::string_view WriteLogMessageAdapter::GetParentAdapterName() const {
  return kWriteLogMessageCommandName;
}

WriteLogMessage::WriteLogMessage(const std::shared_ptr<LogMessageWritable>& adapter) : adapter_(adapter) {
}

void WriteLogMessage::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kLogMessageWritableName));

  const fs::path& log_path = adapter_->GetPath();

  std::ofstream file(log_path, std::ios_base::out | std::ios_base::ate);
  if (!file.is_open())
    throw sw::Exception(std::format("Log '{}' has not been opened.", log_path.generic_string()));

  const std::string& message = adapter_->GetMessage();
  file << message;
  if (message.back() != '\n')
    file << "\n";
}

} // namespace server::commands