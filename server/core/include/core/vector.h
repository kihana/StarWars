#pragma once

#include <cstdint>

namespace server::core {

struct Vector {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
};

} // namespace server::core

server::core::Vector operator+(const server::core::Vector& v1, const server::core::Vector& v2);
