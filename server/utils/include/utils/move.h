#pragma once

#include "core/vector.h"

namespace server::utils {

struct Movable {
  virtual ~Movable() = default;

  virtual const core::Vector& GetPosition() const = 0;
  virtual void SetPosition(const core::Vector& position) = 0;
  virtual const core::Vector& GetVelocity() const = 0;
};

class Move {
public:
  explicit Move(Movable& movable);

  void Execute();

private:
  Movable& movable_;
};

} // namespace server::utils
