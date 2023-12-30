#pragma once

#include <vector>

namespace server::commands {

struct Command {
  virtual ~Command() = default;

  virtual void Execute() = 0;
};

template <typename T> using Queue = std::vector<T>;

} // namespace server::commands
