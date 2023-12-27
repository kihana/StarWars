#include "object.h"

namespace server::core {

std::any& Object::GetValue(std::string_view name) {
  return property_value_by_key_.at(name);
}

void Object::SetValue(std::string_view name, std::any&& value) {
  property_value_by_key_.insert_or_assign(name, std::forward<std::any>(value));
}

} // namespace server::core
