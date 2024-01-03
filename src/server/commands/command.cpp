#include "command.h"

#include "core/object.h"

namespace server::commands {

Adapter::Adapter(const std::shared_ptr<core::Object>& property_holder) : property_holder_(property_holder) {
}

std::any& Adapter::GetAnyValue(std::string_view property_name) {
  if (!property_holder_)
    throw std::runtime_error(std::format("'{}' to get '{}' is unavailable in '{}'.", GetAdapterName(), property_name,
                                         GetParentAdapterName()));
  try {
    auto& any_value = property_holder_->GetValue(property_name);
    if (!any_value.has_value())
      throw std::runtime_error(std::format("'{}' property value is not specified for '{}' object in '{}'.",
                                           property_name, GetAdapterName(), GetParentAdapterName()));

    return any_value;
  } catch (const std::out_of_range&) {
    throw std::runtime_error(std::format("'{}' property is not specified for '{}' object in '{}'.", property_name,
                                         GetAdapterName(), GetParentAdapterName()));
  }
}

} // namespace server::commands