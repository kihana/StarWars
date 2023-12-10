#include "rotate.h"

#include <cmath>

namespace server::utils {

Rotate::Rotate(Rotatable& rotatable) : rotatable_(rotatable) {
}

void Rotate::Execute() {
  const auto pos = rotatable_.GetPosition();
  const auto angle = rotatable_.GetAngle();
  const auto cos_angle = static_cast<std::int64_t>(std::cos(angle));
  const auto sin_angle = static_cast<std::int64_t>(std::sin(angle));
  rotatable_.SetPosition(core::Vector{pos.x * cos_angle - pos.y * sin_angle, pos.x * sin_angle + pos.y * cos_angle});
}

} // namespace server::utils
