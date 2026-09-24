#include "petrocli/stats.h"

#include <gtest/gtest.h>

namespace petrocli {
namespace {

TEST(ComputeDescriptiveStatsTest, ComputesCountMinMaxMean) {
  const DescriptiveStats stats = ComputeDescriptiveStats({1.0, 2.0, 3.0, 4.0});

  EXPECT_EQ(stats.count, 4u);
  ASSERT_TRUE(stats.minimum.has_value());
  ASSERT_TRUE(stats.maximum.has_value());
  ASSERT_TRUE(stats.mean.has_value());
  EXPECT_DOUBLE_EQ(*stats.minimum, 1.0);
  EXPECT_DOUBLE_EQ(*stats.maximum, 4.0);
  EXPECT_DOUBLE_EQ(*stats.mean, 2.5);
}

TEST(ComputeDescriptiveStatsTest, HandlesSingleValue) {
  const DescriptiveStats stats = ComputeDescriptiveStats({42.0});

  EXPECT_EQ(stats.count, 1u);
  EXPECT_DOUBLE_EQ(*stats.minimum, 42.0);
  EXPECT_DOUBLE_EQ(*stats.maximum, 42.0);
  EXPECT_DOUBLE_EQ(*stats.mean, 42.0);
}

TEST(ComputeDescriptiveStatsTest, HandlesNegativeAndUnorderedValues) {
  const DescriptiveStats stats = ComputeDescriptiveStats({-5.0, 10.0, 0.0, -20.0});

  EXPECT_EQ(stats.count, 4u);
  EXPECT_DOUBLE_EQ(*stats.minimum, -20.0);
  EXPECT_DOUBLE_EQ(*stats.maximum, 10.0);
  EXPECT_DOUBLE_EQ(*stats.mean, -3.75);
}

TEST(ComputeDescriptiveStatsTest, ReturnsEmptyFieldsForEmptyInput) {
  const DescriptiveStats stats = ComputeDescriptiveStats({});

  EXPECT_EQ(stats.count, 0u);
  EXPECT_FALSE(stats.minimum.has_value());
  EXPECT_FALSE(stats.maximum.has_value());
  EXPECT_FALSE(stats.mean.has_value());
}

}  // namespace
}  // namespace petrocli
