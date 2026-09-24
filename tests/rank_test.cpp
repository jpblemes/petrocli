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

Sample MakeSampleWithSulfurPct(std::string sample_id, double sulfur_pct) {
  Sample sample;
  sample.sample_id = std::move(sample_id);
  sample.depth_m = 1000;
  sample.api_gravity = 30.0;
  sample.sulfur_pct = sulfur_pct;
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

TEST(RankBySulfurPctTest, OrdersBySulfurPctAscending) {
  const std::vector<Sample> samples = {
      MakeSampleWithSulfurPct("S001", 2.0),
      MakeSampleWithSulfurPct("S002", 0.5),
      MakeSampleWithSulfurPct("S003", 1.0),
  };

  const std::vector<Sample> ranked = RankBySulfurPct(samples, 3);

  ASSERT_EQ(ranked.size(), 3u);
  EXPECT_EQ(ranked[0].sample_id, "S002");
  EXPECT_EQ(ranked[1].sample_id, "S003");
  EXPECT_EQ(ranked[2].sample_id, "S001");
}

TEST(RankBySulfurPctTest, BreaksTiesBySampleIdAscending) {
  const std::vector<Sample> samples = {
      MakeSampleWithSulfurPct("S003", 1.0),
      MakeSampleWithSulfurPct("S001", 1.0),
      MakeSampleWithSulfurPct("S002", 1.0),
  };

  const std::vector<Sample> ranked = RankBySulfurPct(samples, 3);

  ASSERT_EQ(ranked.size(), 3u);
  EXPECT_EQ(ranked[0].sample_id, "S001");
  EXPECT_EQ(ranked[1].sample_id, "S002");
  EXPECT_EQ(ranked[2].sample_id, "S003");
}

TEST(RankBySulfurPctTest, LimitsToFirstNResults) {
  const std::vector<Sample> samples = {
      MakeSampleWithSulfurPct("S001", 2.0),
      MakeSampleWithSulfurPct("S002", 0.5),
      MakeSampleWithSulfurPct("S003", 1.0),
  };

  const std::vector<Sample> ranked = RankBySulfurPct(samples, 2);

  ASSERT_EQ(ranked.size(), 2u);
  EXPECT_EQ(ranked[0].sample_id, "S002");
  EXPECT_EQ(ranked[1].sample_id, "S003");
}

TEST(RankBySulfurPctTest, ReturnsAllWhenLimitExceedsSize) {
  const std::vector<Sample> samples = {MakeSampleWithSulfurPct("S001", 2.0),
                                        MakeSampleWithSulfurPct("S002", 0.5)};
  EXPECT_EQ(RankBySulfurPct(samples, 10).size(), 2u);
}

TEST(RankBySulfurPctTest, ReturnsEmptyWhenLimitIsZero) {
  const std::vector<Sample> samples = {MakeSampleWithSulfurPct("S001", 2.0)};
  EXPECT_TRUE(RankBySulfurPct(samples, 0).empty());
}

TEST(RankBySulfurPctTest, ReturnsEmptyForEmptyInput) {
  EXPECT_TRUE(RankBySulfurPct({}, 5).empty());
}

}  // namespace
}  // namespace petrocli
