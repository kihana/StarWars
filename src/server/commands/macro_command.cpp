#include "macro_command.h"

#include <algorithm>
#include <functional>

namespace ranges = std::ranges;

namespace server::commands {

MacroCommand::MacroCommand(const std::vector<std::shared_ptr<Command>>& commands) : commands_(commands) {
}

void MacroCommand::Execute() {
  ranges::for_each(commands_, std::mem_fn(&Command::Execute));
}

} // namespace server::commands
