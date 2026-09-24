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

}  // namespace
}  // namespace petrocli
