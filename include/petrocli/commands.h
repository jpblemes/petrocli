#pragma once

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

}  // namespace petrocli
