#pragma once

#include <memory>
#include <string_view>

#include "end_command.h"

namespace server::commands {

static std::string_view kEndRotateAdapterName = "End rotate order";
static std::string_view kEndRotateCommandName = "End rotate command";

struct RotateEndable {
  virtual ~RotateEndable() = default;

  virtual std::unique_ptr<Command>& GetRotateCommand() = 0;
  virtual std::weak_ptr<core::Object>& GetRotatable() = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class EndRotateAdapter : public RotateEndable, public EndCommandAdapter {
public:
  explicit EndRotateAdapter(const std::shared_ptr<core::Object>& order);

  void DoEndAction() override;

private:
  std::unique_ptr<Command>& GetRotateCommand() override;
  std::weak_ptr<core::Object>& GetRotatable() override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

} // namespace server::commands
