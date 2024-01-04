#include "vector.h"

namespace server::core {

Vector& Vector::operator+=(const Vector& rhs) noexcept {
  this->x += rhs.x;
  this->y += rhs.y;

  return *this;
}

bool Vector::operator==(const server::core::Vector& that) const noexcept {
  return this->x == that.x && this->y == that.y;
}

} // namespace server::core
