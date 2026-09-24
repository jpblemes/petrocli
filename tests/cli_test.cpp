#include "petrocli/cli.h"

#include <stdexcept>

#include <gtest/gtest.h>

#include "petrocli/commands.h"

namespace petrocli {
namespace {

TEST(ParseArgsTest, ParsesCheckWithInput) {
  const auto command = ParseArgs({"check", "--input", "data.csv"});
  ASSERT_NE(command, nullptr);

  const auto* check_command = dynamic_cast<CheckCommand*>(command.get());
  ASSERT_NE(check_command, nullptr);
  EXPECT_EQ(check_command->input_path(), "data.csv");
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
