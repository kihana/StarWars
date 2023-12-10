#include "rotate.h"

#include <cmath>
#include <numbers>

namespace server::utils {

Rotate::Rotate(Rotatable& rotatable) : rotatable_(rotatable) {
}

void Rotate::Execute() {
  const auto pos = rotatable_.GetPosition();
  const auto angle = rotatable_.GetAngle() * std::numbers::pi / 180;
  rotatable_.SetPosition(core::Vector{std::llround(pos.x * std::cos(angle) - pos.y * std::sin(angle)),
                                      std::llround(pos.x * std::sin(angle) + pos.y * std::cos(angle))});
}

} // namespace server::utils
