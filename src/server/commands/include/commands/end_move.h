#pragma once

#include <memory>

#include <cstdint>

#include "command.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kEndMoveAdapterName = "End move order";
static std::string_view kEndMoveCommandName = "End move command";

struct MoveEndable {
  virtual ~MoveEndable() = default;

  virtual std::unique_ptr<Command>& GetMoveCommand() = 0;
  virtual std::weak_ptr<core::Object>& GetMovable() = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class MoveEndableAdapter : public MoveEndable, public Adapter {
public:
  explicit MoveEndableAdapter(const std::shared_ptr<core::Object>& order);

  std::unique_ptr<Command>& GetMoveCommand() override;
  std::weak_ptr<core::Object>& GetMovable() override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class EndMove : public Command {
public:
  explicit EndMove(const std::shared_ptr<core::Object>& order);
  explicit EndMove(std::unique_ptr<MoveEndable> order);

  void Execute() override;

private:
  std::unique_ptr<MoveEndable> order_;
};

} // namespace server::commands
