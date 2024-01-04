#pragma once

#include <cstdint>

namespace server::core {

struct Vector {
  Vector& operator+=(const Vector& rhs) noexcept;
  friend const Vector operator+(Vector lhs, const Vector& rhs) noexcept {
    lhs += rhs;

    return lhs;
  }

  bool operator==(const server::core::Vector& that) const noexcept;

  Vector() = default;
  Vector(std::int64_t x, std::int64_t y) : x(x), y(y){};

  std::int64_t x = 0;
  std::int64_t y = 0;
};

} // namespace server::core
