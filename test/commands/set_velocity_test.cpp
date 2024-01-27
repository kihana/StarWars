#include <boost/di.hpp>
#include <gtest/gtest.h>

#include "commands/set_velocity.h"
#include "utils.h"

namespace di = boost::di;

namespace server::commands {

TEST(SetVelocityTest, Common) {
  auto velocity_setable = std::make_shared<core::Object>();
  const auto velocity = core::Vector(12, 5);
  const auto injector =
      di::make_injector(di::bind<>().to(velocity_setable), di::bind<VelocitySetable>().to<VelocitySetableAdapter>(),
                        di::bind<core::Vector>().to(velocity));
  auto command = injector.create<SetVelocity>();
  command.Execute();

  EXPECT_EQ(std::any_cast<core::Vector>(velocity_setable->GetValue(kVelocityName)), velocity);
}

TEST(SetVelocityTest, EmptyVelocitySetable) {
  std::shared_ptr<VelocitySetable> empty_velocity_setable;
  const auto velocity = core::Vector(12, 5);
  const auto injector = di::make_injector(
      di::bind<VelocitySetable>().to<VelocitySetableAdapter>(empty_velocity_setable), di::bind<>().to(velocity));
  auto command = injector.create<SetVelocity>();
  EXPECT_ERROR(command, sw::Exception, std::format("'{}' is unavailable.", kSetVelocityAdapterName));
}

TEST(SetVelocityTest, EmptyPropertyHolder) {
  std::shared_ptr<core::Object> velocity_setable;
  const auto velocity = core::Vector(12, 5);
  const auto injector = di::make_injector(di::bind<>().to(velocity_setable),
                                          di::bind<VelocitySetable>().to<VelocitySetableAdapter>()[di::override],
                                          di::bind<>().to(velocity));
  auto command = injector.create<SetVelocity>();
  EXPECT_ERROR(command, sw::Exception,
               std::format("'{}' to set '{}' is unavailable in '{}'.", kSetVelocityAdapterName, kVelocityName,
                           kSetVelocityCommandName));
}

} // namespace server::commands
