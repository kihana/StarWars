#include "command.h"

#include "core/object.h"

namespace server::commands {

Adapter::Adapter(const std::shared_ptr<core::Object>& property_holder) : property_holder_(property_holder) {
}

std::any& Adapter::GetAnyValue(std::string_view property_name) {
  return GetAnyValueImpl(*this, property_name);
}

const std::any& Adapter::GetAnyValue(std::string_view property_name) const {
  return GetAnyValueImpl(*this, property_name);
}

} // namespace server::commands