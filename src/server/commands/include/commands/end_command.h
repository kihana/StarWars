#pragma once

#include <memory>

#include "command.h"

namespace server::commands {

static std::string_view kEndCommandName = "End command";

class EndCommandAdapter : public Adapter {
public:
  explicit EndCommandAdapter(const std::shared_ptr<core::Object>& order);

  virtual void DoEndAction() = 0;
};

class EndCommand final : public Command {
public:
  explicit EndCommand(std::unique_ptr<EndCommandAdapter> adapter);

  void Execute() final;

private:
  std::unique_ptr<EndCommandAdapter> adapter_;
};

} // namespace server::commands
