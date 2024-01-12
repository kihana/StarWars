#pragma once

#include <memory>

#include "command.h"
#include "core/object.h"

namespace server::commands {

static std::string_view kCounterName = "Counter";
static std::string_view kErrorMessageName = "Message";
static std::string_view kRepeatNAdapterName = "NRepeatable";
static std::string_view kRepeatNCommandName = "Repeat N command";

class RepeatNException : public sw::Exception {
public:
  explicit RepeatNException(const std::string& message, const std::string& original_message)
      : sw::Exception(message), original_message_(original_message){};
  explicit RepeatNException(const char* message, const char* original_message)
      : sw::Exception(message), original_message_(original_message){};

  const std::string& GetOriginalMessage() const {
    return original_message_;
  }

private:
  std::string original_message_;
};

struct NRepeatable {
  virtual std::uint8_t GetCounter() const = 0;
  virtual void SetCounter(std::uint8_t value) = 0;
  virtual const std::shared_ptr<Command>& GetRepeatCommand() const = 0;
  virtual const std::string& GetErrorMessage() const = 0;
};

class RepeatNAdapter : public NRepeatable, public Adapter {
public:
  explicit RepeatNAdapter(const std::shared_ptr<core::Object>& n_repeatable);

  std::uint8_t GetCounter() const override;
  void SetCounter(std::uint8_t value) override;
  const std::shared_ptr<Command>& GetRepeatCommand() const override;
  const std::string& GetErrorMessage() const override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class RepeatN : public Command {
public:
  RepeatN(const std::shared_ptr<NRepeatable>& adapter, std::uint8_t n_times);

  void Execute() override;

private:
  std::shared_ptr<NRepeatable> adapter_;
  const std::uint8_t n_times_;
};

} // namespace server::commands
