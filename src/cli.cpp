#include "petrocli/cli.h"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace petrocli {

CliOptions ParseArgs(const std::vector<std::string>& args) {
  if (args.empty()) {
    throw std::runtime_error("usage: petrocli check --input <path>");
  }
  if (args[0] != "check") {
    throw std::runtime_error("unknown command: '" + args[0] + "'");
  }

  CliOptions options;
  options.command = Command::kCheck;

  for (std::size_t i = 1; i < args.size(); ++i) {
    if (args[i] != "--input") {
      throw std::runtime_error("unknown argument: '" + args[i] + "'");
    }
    if (i + 1 >= args.size()) {
      throw std::runtime_error("--input requires a path argument");
    }
    options.input_path = args[++i];
  }

  if (options.input_path.empty()) {
    throw std::runtime_error("usage: petrocli check --input <path>");
  }

  return options;
}

}  // namespace petrocli
