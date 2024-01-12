#include "repeat_n.h"

#include <format>

#include "repeat.h"

namespace server::commands {

RepeatNAdapter::RepeatNAdapter(const std::shared_ptr<core::Object>& n_repeatable) : Adapter(n_repeatable) {
}

std::uint8_t RepeatNAdapter::GetCounter() const {
  const auto& any_counter = GetAnyValue(kCounterName);

  return CastAnyRefToValue<std::uint8_t>(kCounterName, any_counter);
}

void RepeatNAdapter::SetCounter(const std::uint8_t value) {
  SetValue(kCounterName, value);
}

const std::shared_ptr<Command>& RepeatNAdapter::GetRepeatCommand() const {
  const auto& any_command = GetAnyValue(kCommandName);

  return CastAnyRefToRef<std::shared_ptr<Command>>(kCommandName, any_command);
}

const std::string& RepeatNAdapter::GetErrorMessage() const {
  const auto& any_error_message = GetAnyValue(kErrorMessageName);

  return CastAnyRefToRef<std::string>(kErrorMessageName, any_error_message);
}

std::string_view RepeatNAdapter::GetAdapterName() const {
  return kRepeatNAdapterName;
}

std::string_view RepeatNAdapter::GetParentAdapterName() const {
  return kRepeatNCommandName;
}

RepeatN::RepeatN(const std::shared_ptr<NRepeatable>& adapter, const std::uint8_t n_times)
    : adapter_(adapter), n_times_(n_times) {
  adapter_->SetCounter(0);
}

void RepeatN::Execute() {
  if (!adapter_)
    throw sw::Exception(std::format("'{}' is unavailable.", kRepeatNAdapterName));
  auto counter = adapter_->GetCounter();
  if (counter >= n_times_)
    throw RepeatNException(std::format("Command has been executed {} times.", n_times_ + 1),
                           adapter_->GetErrorMessage());

  auto& command_to_repeat = adapter_->GetRepeatCommand();
  adapter_->SetCounter(++counter);
  command_to_repeat->Execute();
}

} // namespace server::commands