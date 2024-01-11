#pragma once

#include <memory>

#include "command.h"
#include "core/object.h"

namespace server::commands {

static std::string_view kCommandName = "Command";
static std::string_view kRepeatAdapterName = "Repeatable";
static std::string_view kRepeatCommandName = "Repeat command";

struct Repeatable {
  virtual const std::shared_ptr<Command>& GetRepeatCommand() const = 0;
  virtual Queue<std::shared_ptr<Command>>& GetCommandQueue() const = 0;
};

class RepeatAdapter : public Repeatable, public Adapter {
public:
  explicit RepeatAdapter(const std::shared_ptr<core::Object>& repeatable);

  const std::shared_ptr<Command>& GetRepeatCommand() const override;
  Queue<std::shared_ptr<Command>>& GetCommandQueue() const override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class Repeat : public Command {
public:
  explicit Repeat(const std::shared_ptr<Repeatable>& adapter);

  void Execute() override;

private:
  std::shared_ptr<Repeatable> adapter_;
};

} // namespace server::commands
