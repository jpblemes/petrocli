#include "petrocli/commands.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

namespace petrocli {
namespace {

void WriteFile(const std::filesystem::path& path, const std::string& content) {
  std::ofstream file(path);
  file << content;
}

class CheckCommandTest : public ::testing::Test {
 protected:
  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_ =
      std::filesystem::temp_directory_path() / "petrocli_check_command_test.csv";
};

TEST_F(CheckCommandTest, PrintsSuccessAndReturnsZeroOnValidFile) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n");

  const CheckCommand command(path_.string());
  std::ostringstream out;
  const int exit_code = command.Execute(out);

  EXPECT_EQ(exit_code, 0);
  EXPECT_NE(out.str().find(path_.string()), std::string::npos);
  EXPECT_NE(out.str().find("1 samples"), std::string::npos);
}

TEST_F(CheckCommandTest, ThrowsOnInvalidFile) {
  WriteFile(path_, "wrong,header,shape\n");

  const CheckCommand command(path_.string());
  std::ostringstream out;
  EXPECT_THROW(command.Execute(out), std::runtime_error);
}

class StatsCommandTest : public ::testing::Test {
 protected:
  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_ =
      std::filesystem::temp_directory_path() / "petrocli_stats_command_test.csv";
};

TEST_F(StatsCommandTest, PrintsCountMinMaxMeanForEachColumn) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.87,ALFA\n"
            "S002,1300,30.2,0.90,0.89,ALFA\n");

  const StatsCommand command(path_.string());
  std::ostringstream out;
  const int exit_code = command.Execute(out);

  EXPECT_EQ(exit_code, 0);
  EXPECT_EQ(out.str(),
            "Column             Count      Min      Max     Mean\n"
            "depth_m                2  1200.00  1300.00  1250.00\n"
            "api_gravity            2    29.80    30.20    30.00\n"
            "sulfur_pct             2     0.90     1.10     1.00\n"
            "density_g_cm3          2     0.87     0.89     0.88\n");
}

TEST_F(StatsCommandTest, PrintsZeroCountForEmptyFile) {
  WriteFile(path_, "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n");

  const StatsCommand command(path_.string());
  std::ostringstream out;
  const int exit_code = command.Execute(out);

  EXPECT_EQ(exit_code, 0);
  EXPECT_EQ(out.str(),
            "Column             Count      Min      Max     Mean\n"
            "depth_m                0        -        -        -\n"
            "api_gravity            0        -        -        -\n"
            "sulfur_pct             0        -        -        -\n"
            "density_g_cm3          0        -        -        -\n");
}

TEST_F(StatsCommandTest, ThrowsOnInvalidFile) {
  WriteFile(path_, "wrong,header,shape\n");

  const StatsCommand command(path_.string());
  std::ostringstream out;
  EXPECT_THROW(command.Execute(out), std::runtime_error);
}

class FilterCommandTest : public ::testing::Test {
 protected:
  void TearDown() override { std::filesystem::remove(path_); }

  std::filesystem::path path_ =
      std::filesystem::temp_directory_path() / "petrocli_filter_command_test.csv";
};

TEST_F(FilterCommandTest, PrintsMatchingSamplesAsAlignedTable) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n"
            "S002,1500,31.2,0.85,0.865,ALFA\n"
            "S003,2000,27.5,1.60,0.892,ALFA\n"
            "S004,1500,34.0,0.45,0.845,BETA\n");

  const FilterCommand command(path_.string(), "ALFA", 1000, 1500);
  std::ostringstream out;
  const int exit_code = command.Execute(out);

  EXPECT_EQ(exit_code, 0);
  EXPECT_EQ(out.str(),
            "sample_id      depth_m   api_gravity   sulfur_pct   density_g_cm3  location\n"
            "S001              1200        29.800        1.100           0.878      ALFA\n"
            "S002              1500        31.200        0.850           0.865      ALFA\n");
}

TEST_F(FilterCommandTest, PrintsHeaderOnlyWhenNoMatches) {
  WriteFile(path_,
            "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location\n"
            "S001,1200,29.8,1.10,0.878,ALFA\n");

  const FilterCommand command(path_.string(), "GAMA", 0, 5000);
  std::ostringstream out;
  const int exit_code = command.Execute(out);

  EXPECT_EQ(exit_code, 0);
  EXPECT_EQ(out.str(),
            "sample_id      depth_m   api_gravity   sulfur_pct   density_g_cm3  location\n");
}

TEST_F(FilterCommandTest, ThrowsOnInvalidFile) {
  WriteFile(path_, "wrong,header,shape\n");

  const FilterCommand command(path_.string(), "ALFA", 0, 5000);
  std::ostringstream out;
  EXPECT_THROW(command.Execute(out), std::runtime_error);
}

}  // namespace
}  // namespace petrocli
