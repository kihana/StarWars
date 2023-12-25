#include "vector.h"

namespace server::core {} // namespace server::core

server::core::Vector operator+(const server::core::Vector v1, const server::core::Vector v2) {
  return server::core::Vector{v1.x + v2.x, v1.y + v2.y};
}

bool operator==(const server::core::Vector v1, const server::core::Vector v2) {
  return v1.x == v2.x && v1.y == v2.y;
}