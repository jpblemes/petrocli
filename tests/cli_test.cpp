#include "petrocli/cli.h"

#include <stdexcept>

#include <gtest/gtest.h>

namespace petrocli {
namespace {

TEST(ParseArgsTest, ParsesCheckWithInput) {
  const CliOptions options = ParseArgs({"check", "--input", "data.csv"});
  EXPECT_EQ(options.command, Command::kCheck);
  EXPECT_EQ(options.input_path, "data.csv");
}

TEST(ParseArgsTest, ThrowsOnNoArguments) {
  EXPECT_THROW(ParseArgs({}), std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnUnknownCommand) {
  EXPECT_THROW(ParseArgs({"bogus", "--input", "data.csv"}), std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnMissingInputFlag) {
  EXPECT_THROW(ParseArgs({"check"}), std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnInputFlagMissingValue) {
  EXPECT_THROW(ParseArgs({"check", "--input"}), std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnUnknownFlag) {
  EXPECT_THROW(ParseArgs({"check", "--bogus", "data.csv"}), std::runtime_error);
}

}  // namespace
}  // namespace petrocli
