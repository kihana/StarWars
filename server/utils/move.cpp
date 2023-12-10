#include "move.h"

namespace server::utils {

core::Vector operator+(const core::Vector& v1, const core::Vector& v2) {
  return core::Vector{v1.x + v2.x, v1.y + v2.y};
}

Move::Move(Movable& movable) : movable_(movable) {
}

void Move::Execute() {
  movable_.SetPosition(movable_.GetPosition() + movable_.GetVelocity());
}

} // namespace server::utils
