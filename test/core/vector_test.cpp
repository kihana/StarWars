#include <gtest/gtest.h>

#include "core/vector.h"

namespace server::core {

TEST(VectorTest, Equals) {
  Vector v1(-3, 4);
  Vector v2(-3, 4);
  Vector v3(-9, 4);

  EXPECT_TRUE(v1 == v2);
  EXPECT_TRUE(v2 == v1);
  EXPECT_FALSE(v1 == v3);
  EXPECT_FALSE(v3 == v1);
  EXPECT_FALSE(v2 == v3);
  EXPECT_FALSE(v3 == v2);
}

TEST(VectorTest, NotEquals) {
  Vector v1(-3, 4);
  Vector v2(-3, 4);
  Vector v3(-9, 4);

  EXPECT_FALSE(v1 != v2);
  EXPECT_FALSE(v2 != v1);
  EXPECT_TRUE(v1 != v3);
  EXPECT_TRUE(v3 != v1);
  EXPECT_TRUE(v2 != v3);
  EXPECT_TRUE(v3 != v2);
}

TEST(VectorTest, Plus) {
  Vector v1(-3, 4);
  Vector v2(-3, 4);
  Vector v3(-9, 4);

  EXPECT_EQ(v1 + v2, Vector(-6, 8));
  EXPECT_EQ(v1 + v3, Vector(-12, 8));
  EXPECT_EQ(v2 + v3, Vector(-12, 8));
  EXPECT_NE(v3 + v2, Vector(-13, 9));
}

} // namespace server::core
