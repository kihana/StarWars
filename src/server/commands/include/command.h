#pragma once

#include <any>
#include <format>
#include <list>
#include <memory>
#include <stdexcept>
#include <string_view>

namespace server::core {
class Object;
}

namespace server::commands {

static std::string_view kCommandQueueName = "CommandQueue";

struct Command {
  virtual ~Command() = default;

  virtual void Execute() = 0;
};

class Adapter {
public:
  explicit Adapter(const std::shared_ptr<core::Object>& property_holder);
  virtual ~Adapter() = default;

protected:
  std::any& GetAnyValue(std::string_view property_name);

  template <typename T> T& CastAnyPointerToRef(std::string_view property_name, std::any& any_value) {
    try {
      return *std::any_cast<T*>(any_value);
    } catch (const std::bad_any_cast&) {
      throw std::runtime_error(std::format("Unexpected type of '{}' property in '{}' object in '{}'.", property_name,
                                           GetAdapterName(), GetParentAdapterName()));
    }
  }

  template <typename T> T& CastAnyRefToRef(std::string_view property_name, std::any& any_value) {
    try {
      return std::any_cast<T&>(any_value);
    } catch (const std::bad_any_cast&) {
      throw std::runtime_error(std::format("Unexpected type of '{}' property in '{}' object in '{}'.", property_name,
                                           GetAdapterName(), GetParentAdapterName()));
    }
  }

private:
  virtual std::string_view GetAdapterName() const = 0;
  virtual std::string_view GetParentAdapterName() const = 0;

protected:
  std::shared_ptr<core::Object> property_holder_;
};

template <typename T> using Queue = std::list<T>;

} // namespace server::commands
