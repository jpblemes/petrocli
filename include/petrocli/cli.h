#pragma once

#include <string>
#include <vector>

namespace petrocli {

enum class Command {
  kUnknown,
  kCheck,
};

/** Parsed command-line arguments. */
struct CliOptions {
  Command command = Command::kUnknown;
  std::string input_path;
};

/** Parses `args` (excluding the program name); throws
 *  std::runtime_error on invalid usage. */
CliOptions ParseArgs(const std::vector<std::string>& args);

}  // namespace petrocli
