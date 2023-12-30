#pragma once

#include <memory>

#include <cstdint>

#include "command.h"
#include "core/vector.h"

namespace server::core {
class Object;
}

namespace server::commands {

struct MoveCommandStartable {
  virtual ~MoveCommandStartable() = default;

  virtual std::weak_ptr<core::Object>& GetMovable() = 0;
  virtual core::Vector GetVelocity() = 0;
  virtual Queue<std::unique_ptr<Command>>& GetCommandQueue() = 0;
};

class StartMove : public Command {
public:
  explicit StartMove(const std::shared_ptr<MoveCommandStartable>& move_startable, core::Vector velocity);

  void Execute() override;

private:
  std::weak_ptr<MoveCommandStartable> move_startable_;
  core::Vector velocity_;
};

} // namespace server::commands
