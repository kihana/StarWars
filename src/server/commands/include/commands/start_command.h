#pragma once

#include <memory>

#include "command.h"

namespace server::commands {

static std::string_view kStartCommandName = "Start command";

class StartCommandAdapter : public Adapter {
public:
  explicit StartCommandAdapter(const std::shared_ptr<core::Object>& order);

  virtual void DoStartAction() = 0;
};

class StartCommand final : public Command {
public:
  explicit StartCommand(std::unique_ptr<StartCommandAdapter> adapter);

  void Execute() final;

private:
  std::unique_ptr<StartCommandAdapter> adapter_;
};

} // namespace server::commands
