#include "petrocli/commands.h"

#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "petrocli/csv.h"

namespace petrocli {

std::unique_ptr<Command> CheckCommand::Parse(const std::vector<std::string>& args) {
  std::string input_path;

  for (std::size_t i = 0; i < args.size(); ++i) {
    if (args[i] != "--input") {
      throw std::runtime_error("unknown argument: '" + args[i] + "'");
    }
    if (i + 1 >= args.size()) {
      throw std::runtime_error("--input requires a path argument");
    }
    input_path = args[++i];
  }

  if (input_path.empty()) {
    throw std::runtime_error("usage: petrocli check --input <path>");
  }

  return std::make_unique<CheckCommand>(std::move(input_path));
}

CheckCommand::CheckCommand(std::string input_path) : input_path_(std::move(input_path)) {}

int CheckCommand::Execute(std::ostream& out) const {
  const std::vector<Sample> samples = ReadCsv(input_path_);
  out << "valid: " << input_path_ << " (" << samples.size() << " samples)\n";
  return 0;
}

}  // namespace petrocli
