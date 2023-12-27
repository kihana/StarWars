#include <gtest/gtest.h>

#include "core/object.h"
#include "core/vector.h"

namespace server::core {

std::string_view kExistentKey = "ExistentKey";
std::string_view kNonExistentKey = "NonExistentKey";

class ObjectTest : public testing::Test {
protected:
  void SetUp() override {
    object_.SetValue(kExistentKey, std::make_any<Vector>(-3, 4));
  }

  Object object_;
};

TEST_F(ObjectTest, GetValueForExistentKey) {
  const auto& any_value = object_.GetValue(kExistentKey);
  EXPECT_TRUE(any_value.has_value());
}

TEST_F(ObjectTest, GetValueForCorrectValueType) {
  const auto& any_value = object_.GetValue(kExistentKey);
  ASSERT_TRUE(any_value.has_value());

  const auto& vector_value = std::any_cast<const Vector&>(any_value);
  EXPECT_TRUE(Vector(-3, 4) == vector_value);
}

TEST_F(ObjectTest, GetValueForNonExistentKey) {
  EXPECT_THROW(object_.GetValue(kNonExistentKey), std::out_of_range);
}

TEST_F(ObjectTest, GetValueForIncorrectValueType) {
  const auto& any_value = object_.GetValue(kExistentKey);
  ASSERT_TRUE(any_value.has_value());

  EXPECT_THROW(
      {
        const auto& int_value = std::any_cast<const int&>(any_value);
        int_value;
      },
      std::bad_any_cast);
}

TEST_F(ObjectTest, SetValueForExistentKeyWithTheSameType) {
  object_.SetValue(kExistentKey, std::make_any<Vector>(122, 133));
  const auto& any_value = object_.GetValue(kExistentKey);
  ASSERT_TRUE(any_value.has_value());

  const auto& vector_value = std::any_cast<const Vector&>(any_value);
  EXPECT_TRUE(Vector(122, 133) == vector_value);
}

TEST_F(ObjectTest, SetValueForExistentKeyWithNewType) {
  object_.SetValue(kExistentKey, std::make_any<int>(5));
  const auto& any_value = object_.GetValue(kExistentKey);
  ASSERT_TRUE(any_value.has_value());

  const auto int_value = std::any_cast<int>(any_value);
  EXPECT_TRUE(5 == int_value);
}

TEST_F(ObjectTest, SetValueForNonExistentKey) {
  object_.SetValue(kNonExistentKey, std::make_any<int>(5));
  const auto& any_value = object_.GetValue(kNonExistentKey);
  ASSERT_TRUE(any_value.has_value());

  const auto int_value = std::any_cast<int>(any_value);
  EXPECT_TRUE(5 == int_value);
}

} // namespace server::core
