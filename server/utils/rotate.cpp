#include "rotate.h"

#include <cmath>

namespace server::utils {

Rotate::Rotate(Rotatable& rotatable) : rotatable_(rotatable) {
}

void Rotate::Execute() {
  const auto current_position = rotatable_.GetPosition();
  const auto angle = rotatable_.GetAngle();
  core::Vector new_position(current_position.x * std::cos(angle) - current_position.y * std::sin(angle),
                            current_position.x * std::sin(angle) + current_position.y * std::cos(angle));
  rotatable_.SetPosition(new_position);
}

} // namespace server::utils
