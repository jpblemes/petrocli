#include "petrocli/validation.h"

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "petrocli/model.h"

namespace petrocli {
namespace {

Sample ValidSample() {
  return Sample{"S001", 1200.0, 29.8, 1.10, 0.878, "ALFA"};
}

TEST(IsValidHeaderTest, AcceptsExpectedHeader) {
  const std::vector<std::string> header = {"sample_id",  "depth_m",       "api_gravity",
                                            "sulfur_pct", "density_g_cm3", "location"};
  EXPECT_TRUE(IsValidHeader(header));
}

TEST(IsValidHeaderTest, RejectsWrongOrder) {
  const std::vector<std::string> header = {"depth_m",    "sample_id",     "api_gravity",
                                            "sulfur_pct", "density_g_cm3", "location"};
  EXPECT_FALSE(IsValidHeader(header));
}

TEST(IsValidHeaderTest, RejectsWrongColumnCount) {
  const std::vector<std::string> header = {"sample_id", "depth_m"};
  EXPECT_FALSE(IsValidHeader(header));
}

TEST(ValidateFieldCountTest, AcceptsSixFields) {
  EXPECT_NO_THROW(ValidateFieldCount({"a", "b", "c", "d", "e", "f"}));
}

TEST(ValidateFieldCountTest, ThrowsOnTooFewFields) {
  EXPECT_THROW(ValidateFieldCount({"a", "b"}), std::runtime_error);
}

TEST(ValidateFieldCountTest, ThrowsOnTooManyFields) {
  EXPECT_THROW(ValidateFieldCount({"a", "b", "c", "d", "e", "f", "g"}), std::runtime_error);
}

TEST(ValidateNumericFieldTest, ParsesValidNumber) {
  EXPECT_DOUBLE_EQ(ValidateNumericField("29.8", "api_gravity"), 29.8);
}

TEST(ValidateNumericFieldTest, ThrowsOnNonNumericText) {
  EXPECT_THROW(ValidateNumericField("not-a-number", "api_gravity"), std::runtime_error);
}

TEST(ValidateNumericFieldTest, ThrowsOnTrailingGarbage) {
  EXPECT_THROW(ValidateNumericField("29.8x", "api_gravity"), std::runtime_error);
}

TEST(ValidateNumericFieldTest, ThrowsOnInfinity) {
  EXPECT_THROW(ValidateNumericField("inf", "api_gravity"), std::runtime_error);
  EXPECT_THROW(ValidateNumericField("-inf", "api_gravity"), std::runtime_error);
}

TEST(ValidateNumericFieldTest, ThrowsOnNaN) {
  EXPECT_THROW(ValidateNumericField("nan", "api_gravity"), std::runtime_error);
}

TEST(ValidateSampleTest, AcceptsValidSample) {
  EXPECT_NO_THROW(ValidateSample(ValidSample()));
}

TEST(ValidateSampleTest, RejectsNonFiniteDepth) {
  Sample sample = ValidSample();
  sample.depth_m = std::numeric_limits<double>::infinity();
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, RejectsNonFiniteApiGravity) {
  Sample sample = ValidSample();
  sample.api_gravity = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, RejectsNonFiniteSulfurPct) {
  Sample sample = ValidSample();
  sample.sulfur_pct = -std::numeric_limits<double>::infinity();
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, RejectsNonFiniteDensity) {
  Sample sample = ValidSample();
  sample.density_g_cm3 = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, RejectsNegativeDepth) {
  Sample sample = ValidSample();
  sample.depth_m = -1.0;
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, RejectsFractionalDepth) {
  Sample sample = ValidSample();
  sample.depth_m = 1200.5;
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

TEST(ValidateSampleTest, AcceptsZeroDepth) {
  Sample sample = ValidSample();
  sample.depth_m = 0.0;
  EXPECT_NO_THROW(ValidateSample(sample));
}

TEST(ValidateSampleTest, RejectsEmptyLocation) {
  Sample sample = ValidSample();
  sample.location = "";
  EXPECT_THROW(ValidateSample(sample), std::runtime_error);
}

}  // namespace
}  // namespace petrocli
