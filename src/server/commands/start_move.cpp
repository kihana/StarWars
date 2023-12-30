#include "start_move.h"

#include <stdexcept>

#include "move.h"
#include "set_velocity.h"

namespace server::commands {

StartMove::StartMove(const std::shared_ptr<MoveCommandStartable>& move_startable, const core::Vector velocity)
    : move_startable_(move_startable), velocity_(velocity) {
}

void StartMove::Execute() {
  auto order = move_startable_.lock();
  if (!order)
    throw std::runtime_error("Start move order is unavailable.");

  std::shared_ptr<core::Object> movable = order->GetMovable().lock();
  if (!movable)
    throw std::runtime_error("Object to start moving is unavailable.");

  auto& command_queue = order->GetCommandQueue();

  command_queue.push_back(std::move(std::make_unique<SetVelocity>(movable, velocity_)));
  command_queue.push_back(std::move(std::make_unique<Move>(movable)));
}

} // namespace server::commands
