#pragma once

#include <memory>
#include <vector>

#include "command.h"

namespace server::commands {

class MacroCommand : public Command {
public:
  explicit MacroCommand(const std::vector<std::shared_ptr<Command>>& commands);

  void Execute() override;

private:
  std::vector<std::shared_ptr<Command>> commands_;
};

} // namespace server::commands
