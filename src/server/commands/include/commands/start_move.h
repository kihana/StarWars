#pragma once

#include <memory>
#include <string_view>

#include "start_command.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kStartMoveAdapterName = "Start move order";
static std::string_view kStartMoveCommandName = "Start move command";

struct MoveStartable {
  virtual ~MoveStartable() = default;

  virtual std::weak_ptr<core::Object>& GetMovable() = 0;
  virtual core::Vector GetVelocity() const = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class StartMoveAdapter : public MoveStartable, public StartCommandAdapter {
public:
  explicit StartMoveAdapter(const std::shared_ptr<core::Object>& order);

  void DoStartAction() override;

private:
  std::weak_ptr<core::Object>& GetMovable() override;
  core::Vector GetVelocity() const override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

} // namespace server::commands
