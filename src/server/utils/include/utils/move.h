#pragma once

#include "core/vector.h"

namespace server::utils {

struct Movable {
  virtual ~Movable() = default;

  virtual core::Vector GetPosition() const = 0;
  virtual void SetPosition(core::Vector position) = 0;
  virtual core::Vector GetVelocity() const = 0;
  virtual void SetVelocity(core::Vector velocity) = 0;
};

class Move {
public:
  explicit Move(Movable& movable);

  void Execute();

private:
  Movable& movable_;
};

} // namespace server::utils
