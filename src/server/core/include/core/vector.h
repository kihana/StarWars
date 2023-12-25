#pragma once

#include <cstdint>

namespace server::core {

struct Vector {
  Vector() = default;
  Vector(std::int64_t x, std::int64_t y) : x(x), y(y){};

  std::int64_t x = 0;
  std::int64_t y = 0;
};

} // namespace server::core

server::core::Vector operator+(server::core::Vector v1, server::core::Vector v2);
bool operator==(server::core::Vector v1, server::core::Vector v2);
