#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "petrocli/command.h"

namespace petrocli {

/** Validates a CSV file via ReadCsv and reports the result. */
class CheckCommand : public Command {
 public:
  /** Parses `args` into a CheckCommand; throws std::runtime_error
   *  on invalid usage. */
  static std::unique_ptr<Command> Parse(const std::vector<std::string>& args);

  explicit CheckCommand(std::string input_path);

  int Execute(std::ostream& out) const override;

  const std::string& input_path() const { return input_path_; }

 private:
  std::string input_path_;
};

/** Prints count, min, max, and mean for depth_m, api_gravity,
 *  sulfur_pct, and density_g_cm3. */
class StatsCommand : public Command {
 public:
  /** Parses `args` into a StatsCommand; throws std::runtime_error
   *  on invalid usage. */
  static std::unique_ptr<Command> Parse(const std::vector<std::string>& args);

  explicit StatsCommand(std::string input_path);

  int Execute(std::ostream& out) const override;

  const std::string& input_path() const { return input_path_; }

 private:
  std::string input_path_;
};

/** Prints, as CSV, the samples matching a location and depth range. */
class FilterCommand : public Command {
 public:
  /** Parses `args` into a FilterCommand; throws std::runtime_error
   *  on invalid usage. */
  static std::unique_ptr<Command> Parse(const std::vector<std::string>& args);

  FilterCommand(std::string input_path, std::string location, double depth_min, double depth_max);

  int Execute(std::ostream& out) const override;

  const std::string& input_path() const { return input_path_; }
  const std::string& location() const { return location_; }
  double depth_min() const { return depth_min_; }
  double depth_max() const { return depth_max_; }

 private:
  std::string input_path_;
  std::string location_;
  double depth_min_;
  double depth_max_;
};

/** Which numeric field to rank samples by. */
enum class RankBy {
  kApiGravity,
  kSulfurPct,
};

/** Prints, as a simple table, the top-N samples ranked by
 *  api_gravity or sulfur_pct. */
class RankCommand : public Command {
 public:
  /** Parses `args` into a RankCommand; throws std::runtime_error
   *  on invalid usage. */
  static std::unique_ptr<Command> Parse(const std::vector<std::string>& args);

  RankCommand(std::string input_path, RankBy rank_by, std::size_t top);

  int Execute(std::ostream& out) const override;

  const std::string& input_path() const { return input_path_; }
  RankBy rank_by() const { return rank_by_; }
  std::size_t top() const { return top_; }

 private:
  std::string input_path_;
  RankBy rank_by_;
  std::size_t top_;
};

}  // namespace petrocli
