#pragma once

#include "core/angle.h"
#include "core/vector.h"

namespace server::commands {

struct Rotatable {
  virtual ~Rotatable() = default;

  virtual core::Vector GetPosition() const = 0;
  virtual void SetPosition(core::Vector position) = 0;
  virtual core::Angle GetAngle() const = 0;
  virtual void SetAngle(core::Angle angle) = 0;
};

class Rotate {
public:
  explicit Rotate(Rotatable& rotatable);

  void Execute();

private:
  Rotatable& rotatable_;
};

} // namespace server::commands
