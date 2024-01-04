#pragma once

#include <any>
#include <string_view>
#include <unordered_map>

namespace server::core {

class Object {
public:
  std::any& GetValue(std::string_view name);
  void SetValue(std::string_view name, std::any&& value);
  void RemoveKey(std::string_view name);

private:
  std::unordered_map<std::string_view, std::any> property_value_by_key_;
};

} // namespace server::core
