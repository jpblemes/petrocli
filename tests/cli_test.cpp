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

TEST(ParseArgsTest, ParsesStatsWithInput) {
  const auto command = ParseArgs({"stats", "--input", "data.csv"});
  ASSERT_NE(command, nullptr);

  const auto* stats_command = dynamic_cast<StatsCommand*>(command.get());
  ASSERT_NE(stats_command, nullptr);
  EXPECT_EQ(stats_command->input_path(), "data.csv");
}

TEST(ParseArgsTest, ParsesFilterWithAllFlags) {
  const auto command =
      ParseArgs({"filter", "--input", "data.csv", "--location", "ALFA", "--depth-min", "1000",
                 "--depth-max", "1500"});
  ASSERT_NE(command, nullptr);

  const auto* filter_command = dynamic_cast<FilterCommand*>(command.get());
  ASSERT_NE(filter_command, nullptr);
  EXPECT_EQ(filter_command->input_path(), "data.csv");
  EXPECT_EQ(filter_command->location(), "ALFA");
  EXPECT_DOUBLE_EQ(filter_command->depth_min(), 1000.0);
  EXPECT_DOUBLE_EQ(filter_command->depth_max(), 1500.0);
}

TEST(ParseArgsTest, ThrowsOnFilterMissingFlag) {
  EXPECT_THROW(ParseArgs({"filter", "--input", "data.csv", "--location", "ALFA"}),
               std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnFilterInvalidDepthValue) {
  EXPECT_THROW(ParseArgs({"filter", "--input", "data.csv", "--location", "ALFA", "--depth-min",
                          "abc", "--depth-max", "1500"}),
               std::runtime_error);
}

TEST(ParseArgsTest, ParsesRankWithAllFlags) {
  const auto command = ParseArgs({"rank", "--input", "data.csv", "--by", "api", "--top", "5"});
  ASSERT_NE(command, nullptr);

  const auto* rank_command = dynamic_cast<RankCommand*>(command.get());
  ASSERT_NE(rank_command, nullptr);
  EXPECT_EQ(rank_command->input_path(), "data.csv");
  EXPECT_EQ(rank_command->rank_by(), RankBy::kApiGravity);
  EXPECT_EQ(rank_command->top(), 5u);
}

TEST(ParseArgsTest, ParsesRankBySulfur) {
  const auto command = ParseArgs({"rank", "--input", "data.csv", "--by", "sulfur", "--top", "3"});
  ASSERT_NE(command, nullptr);

  const auto* rank_command = dynamic_cast<RankCommand*>(command.get());
  ASSERT_NE(rank_command, nullptr);
  EXPECT_EQ(rank_command->rank_by(), RankBy::kSulfurPct);
}

TEST(ParseArgsTest, ThrowsOnRankMissingFlag) {
  EXPECT_THROW(ParseArgs({"rank", "--input", "data.csv", "--by", "api"}), std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnRankInvalidBy) {
  EXPECT_THROW(
      ParseArgs({"rank", "--input", "data.csv", "--by", "bogus", "--top", "5"}),
      std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnRankNonPositiveTop) {
  EXPECT_THROW(ParseArgs({"rank", "--input", "data.csv", "--by", "api", "--top", "0"}),
               std::runtime_error);
}

TEST(ParseArgsTest, ThrowsOnRankFractionalTop) {
  EXPECT_THROW(ParseArgs({"rank", "--input", "data.csv", "--by", "api", "--top", "2.5"}),
               std::runtime_error);
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
