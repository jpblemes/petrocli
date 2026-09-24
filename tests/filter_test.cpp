#include "petrocli/filter.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "petrocli/model.h"

namespace petrocli {
namespace {

Sample MakeSample(std::string sample_id, double depth_m, std::string location) {
  Sample sample;
  sample.sample_id = std::move(sample_id);
  sample.depth_m = depth_m;
  sample.api_gravity = 30.0;
  sample.sulfur_pct = 1.0;
  sample.density_g_cm3 = 0.87;
  sample.location = std::move(location);
  return sample;
}

TEST(FilterSamplesTest, ReturnsMatchingLocationAndDepthRange) {
  const std::vector<Sample> samples = {
      MakeSample("S001", 1000, "ALFA"),
      MakeSample("S002", 1500, "ALFA"),
      MakeSample("S003", 2000, "ALFA"),
      MakeSample("S004", 1500, "BETA"),
  };

  const std::vector<Sample> filtered = FilterSamples(samples, "ALFA", 1000, 1500);

  ASSERT_EQ(filtered.size(), 2u);
  EXPECT_EQ(filtered[0].sample_id, "S001");
  EXPECT_EQ(filtered[1].sample_id, "S002");
}

TEST(FilterSamplesTest, ExcludesWrongLocation) {
  const std::vector<Sample> samples = {MakeSample("S001", 1500, "BETA")};
  EXPECT_TRUE(FilterSamples(samples, "ALFA", 0, 5000).empty());
}

TEST(FilterSamplesTest, ExcludesDepthOutsideRange) {
  const std::vector<Sample> samples = {
      MakeSample("S001", 999, "ALFA"),
      MakeSample("S002", 1501, "ALFA"),
  };
  EXPECT_TRUE(FilterSamples(samples, "ALFA", 1000, 1500).empty());
}

TEST(FilterSamplesTest, IncludesBoundaryDepths) {
  const std::vector<Sample> samples = {
      MakeSample("S001", 1000, "ALFA"),
      MakeSample("S002", 1500, "ALFA"),
  };

  EXPECT_EQ(FilterSamples(samples, "ALFA", 1000, 1500).size(), 2u);
}

TEST(FilterSamplesTest, ReturnsEmptyForEmptyInput) {
  EXPECT_TRUE(FilterSamples({}, "ALFA", 0, 5000).empty());
}

}  // namespace
}  // namespace petrocli
