#include "petrocli/commands.h"

#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "petrocli/csv.h"

namespace petrocli {

namespace {

int RunCheck(const std::string& input_path, std::ostream& out) {
  const std::vector<Sample> samples = ReadCsv(input_path);
  out << "valid: " << input_path << " (" << samples.size() << " samples)\n";
  return 0;
}

}  // namespace

int RunCommand(const CliOptions& options, std::ostream& out) {
  switch (options.command) {
    case Command::kCheck:
      return RunCheck(options.input_path, out);
    case Command::kUnknown:
      break;
  }
  throw std::runtime_error("no command specified");
}

}  // namespace petrocli
