#include "petrocli/csv.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "petrocli/model.h"

namespace petrocli {
namespace {

void WriteFile(const std::filesystem::path& path, const std::string& content) {
  std::ofstream file(path);
  file << content;
}

class ReadCsvTest : public ::testing::Test {
 protected:
  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_ =
      std::filesystem::temp_directory_path() / "petrocli_read_csv_test.csv";
};

TEST(SplitCsvLineTest, SplitsFields) {
  EXPECT_EQ(SplitCsvLine("a,b,c"), (std::vector<std::string>{"a", "b", "c"}));
}

TEST(SplitCsvLineTest, KeepsTrailingEmptyField) {
  EXPECT_EQ(SplitCsvLine("a,b,"), (std::vector<std::string>{"a", "b", ""}));
}

TEST(SplitCsvLineTest, SingleFieldWithNoCommas) {
  EXPECT_EQ(SplitCsvLine("a"), (std::vector<std::string>{"a"}));
}

TEST(ParseSampleFieldsTest, ParsesValidRow) {
  const Sample sample = ParseSampleFields({"S001", "1200", "29.8", "1.10", "0.878", "ALFA"});

  EXPECT_EQ(sample.sample_id, "S001");
  EXPECT_DOUBLE_EQ(sample.depth_m, 1200.0);
  EXPECT_DOUBLE_EQ(sample.api_gravity, 29.8);
  EXPECT_DOUBLE_EQ(sample.sulfur_pct, 1.10);
  EXPECT_DOUBLE_EQ(sample.density_g_cm3, 0.878);
  EXPECT_EQ(sample.location, "ALFA");
}

TEST(ParseSampleFieldsTest, ThrowsOnWrongFieldCount) {
  EXPECT_THROW(ParseSampleFields({"S001", "1200"}), std::runtime_error);
}

TEST(ParseSampleFieldsTest, ThrowsOnInvalidNumericField) {
  // Exhaustive coverage lives in validation_test.cpp; this checks wiring.
  EXPECT_THROW(ParseSampleFields({"S001", "1200", "not-a-number", "1.10", "0.878", "ALFA"}),
               std::runtime_error);
}

TEST(ParseSampleFieldsTest, ThrowsOnInvalidSampleValue) {
  // Exhaustive coverage lives in validation_test.cpp; this checks wiring.
  EXPECT_THROW(ParseSampleFields({"S001", "1200", "29.8", "1.10", "0.878", ""}),
               std::runtime_error);
}

TEST_F(ReadCsvTest, ParsesAllRows) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n"
            "S002,1250,31.2,0.85,0.865,ALFA\n");

  const std::vector<Sample> samples = ReadCsv(path_.string());

  ASSERT_EQ(samples.size(), 2u);
  EXPECT_EQ(samples[0].sample_id, "S001");
  EXPECT_EQ(samples[1].sample_id, "S002");
  EXPECT_EQ(samples[1].location, "ALFA");
}

TEST_F(ReadCsvTest, HandlesCrlfLineEndings) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\r\n"
            "S001,1200,29.8,1.10,0.878,ALFA\r\n"
            "S002,1250,31.2,0.85,0.865,ALFA\r\n");

  const std::vector<Sample> samples = ReadCsv(path_.string());

  ASSERT_EQ(samples.size(), 2u);
  EXPECT_EQ(samples[0].location, "ALFA");
  EXPECT_EQ(samples[1].location, "ALFA");
}

TEST_F(ReadCsvTest, SkipsBlankLines) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n"
            "\n");

  EXPECT_EQ(ReadCsv(path_.string()).size(), 1u);
}

TEST_F(ReadCsvTest, ThrowsOnMissingFile) {
  const std::filesystem::path missing = path_.parent_path() / "petrocli_does_not_exist.csv";
  EXPECT_THROW(ReadCsv(missing.string()), std::runtime_error);
}

TEST_F(ReadCsvTest, ThrowsOnEmptyFile) {
  WriteFile(path_, "");
  EXPECT_THROW(ReadCsv(path_.string()), std::runtime_error);
}

TEST_F(ReadCsvTest, ThrowsOnInvalidHeader) {
  WriteFile(path_, "id,depth,api,sulfur,density,loc\n");
  EXPECT_THROW(ReadCsv(path_.string()), std::runtime_error);
}

TEST_F(ReadCsvTest, ErrorMessageIncludesLineNumber) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n"
            "S002,1250,bad,0.85,0.865,ALFA\n");

  try {
    ReadCsv(path_.string());
    FAIL() << "expected ReadCsv to throw";
  } catch (const std::runtime_error& e) {
    EXPECT_NE(std::string(e.what()).find(":3:"), std::string::npos);
  }
}

}  // namespace
}  // namespace petrocli
