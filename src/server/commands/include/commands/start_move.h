#pragma once

#include <memory>
#include <string_view>

#include <cstdint>

#include "command.h"
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

class MoveStartableAdapter : public MoveStartable, public Adapter {
public:
  explicit MoveStartableAdapter(const std::shared_ptr<core::Object>& order);

  std::weak_ptr<core::Object>& GetMovable() override;
  core::Vector GetVelocity() const override;
  Queue<std::unique_ptr<Command>>& GetCommandQueue() override;

private:
  std::string_view GetAdapterName() const override;
  std::string_view GetParentAdapterName() const override;
};

class StartMove : public Command {
public:
  explicit StartMove(const std::shared_ptr<core::Object>& order);
  explicit StartMove(std::unique_ptr<MoveStartable> order);

  void Execute() override;

private:
  std::unique_ptr<MoveStartable> order_;
};

} // namespace server::commands
