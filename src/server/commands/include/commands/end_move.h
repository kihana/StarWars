#pragma once

#include <memory>
#include <string_view>

#include "end_command.h"

namespace server::commands {

static std::string_view kEndMoveAdapterName = "End move order";
static std::string_view kEndMoveCommandName = "End move command";

struct MoveEndable {
  virtual ~MoveEndable() = default;

  virtual std::unique_ptr<Command>& GetMoveCommand() = 0;
  virtual std::weak_ptr<core::Object>& GetMovable() = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class EndMoveAdapter : public MoveEndable, public EndCommandAdapter {
public:
  explicit EndMoveAdapter(const std::shared_ptr<core::Object>& order);

  void DoEndAction() override;

private:
  std::unique_ptr<Command>& GetMoveCommand() override;
  std::weak_ptr<core::Object>& GetMovable() override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

} // namespace server::commands
