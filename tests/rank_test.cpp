#include "petrocli/rank.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "petrocli/model.h"

namespace petrocli {
namespace {

Sample MakeSample(std::string sample_id, double api_gravity) {
  Sample sample;
  sample.sample_id = std::move(sample_id);
  sample.depth_m = 1000;
  sample.api_gravity = api_gravity;
  sample.sulfur_pct = 1.0;
  sample.density_g_cm3 = 0.87;
  sample.location = "ALFA";
  return sample;
}

TEST(RankByApiGravityTest, OrdersByApiGravityDescending) {
  const std::vector<Sample> samples = {
      MakeSample("S001", 25.0),
      MakeSample("S002", 35.0),
      MakeSample("S003", 30.0),
  };

  const std::vector<Sample> ranked = RankByApiGravity(samples, 3);

  ASSERT_EQ(ranked.size(), 3u);
  EXPECT_EQ(ranked[0].sample_id, "S002");
  EXPECT_EQ(ranked[1].sample_id, "S003");
  EXPECT_EQ(ranked[2].sample_id, "S001");
}

TEST(RankByApiGravityTest, BreaksTiesBySampleIdAscending) {
  const std::vector<Sample> samples = {
      MakeSample("S003", 30.0),
      MakeSample("S001", 30.0),
      MakeSample("S002", 30.0),
  };

  const std::vector<Sample> ranked = RankByApiGravity(samples, 3);

  ASSERT_EQ(ranked.size(), 3u);
  EXPECT_EQ(ranked[0].sample_id, "S001");
  EXPECT_EQ(ranked[1].sample_id, "S002");
  EXPECT_EQ(ranked[2].sample_id, "S003");
}

TEST(RankByApiGravityTest, LimitsToFirstNResults) {
  const std::vector<Sample> samples = {
      MakeSample("S001", 25.0),
      MakeSample("S002", 35.0),
      MakeSample("S003", 30.0),
  };

  const std::vector<Sample> ranked = RankByApiGravity(samples, 2);

  ASSERT_EQ(ranked.size(), 2u);
  EXPECT_EQ(ranked[0].sample_id, "S002");
  EXPECT_EQ(ranked[1].sample_id, "S003");
}

TEST(RankByApiGravityTest, ReturnsAllWhenLimitExceedsSize) {
  const std::vector<Sample> samples = {MakeSample("S001", 25.0), MakeSample("S002", 35.0)};
  EXPECT_EQ(RankByApiGravity(samples, 10).size(), 2u);
}

TEST(RankByApiGravityTest, ReturnsEmptyWhenLimitIsZero) {
  const std::vector<Sample> samples = {MakeSample("S001", 25.0)};
  EXPECT_TRUE(RankByApiGravity(samples, 0).empty());
}

TEST(RankByApiGravityTest, ReturnsEmptyForEmptyInput) {
  EXPECT_TRUE(RankByApiGravity({}, 5).empty());
}

}  // namespace
}  // namespace petrocli
