#pragma once

#include <memory>

#include "command.h"
#include "core/object.h"
#include "core/vector.h"

namespace server::commands {

static std::string_view kPositionName = "Position";

struct Movable {
  virtual ~Movable() = default;

  virtual core::Vector GetPosition() const = 0;
  virtual void SetPosition(core::Vector value) = 0;
  virtual core::Vector GetVelocity() const = 0;
};

class MovableAdapter : public Movable {
public:
  explicit MovableAdapter(const std::shared_ptr<core::Object>& movable);

  core::Vector GetPosition() const override;
  void SetPosition(core::Vector value) override;
  core::Vector GetVelocity() const override;

private:
  std::weak_ptr<core::Object> movable_;
};

class Move : public Command {
public:
  explicit Move(const std::shared_ptr<core::Object>& movable);

  void Execute();

private:
  std::unique_ptr<Movable> movable_;
};

} // namespace server::commands
