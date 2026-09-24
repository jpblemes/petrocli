#include "petrocli/cli.h"

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "petrocli/commands.h"

namespace petrocli {

namespace {

using CommandFactory = std::function<std::unique_ptr<Command>(const std::vector<std::string>&)>;

const std::map<std::string, CommandFactory>& CommandFactories() {
  static const std::map<std::string, CommandFactory> kFactories = {
      {"check", &CheckCommand::Parse},
      {"filter", &FilterCommand::Parse},
      {"rank", &RankCommand::Parse},
      {"stats", &StatsCommand::Parse},
  };
  return kFactories;
}

bool IsHelpFlag(const std::string& arg) {
  return arg == "--help" || arg == "-h" || arg == "help";
}

std::string GlobalHelpText() {
  return std::string("petrocli: validate, summarize, filter, and rank petroleum sample CSV data.\n") +
         "\n" +
         "Commands:\n" +
         "  check   Validate a CSV file.\n" +
         "          " + CheckCommand::Usage() + "\n" +
         "\n" +
         "  stats   Print count, min, max, and mean for each numeric column.\n" +
         "          " + StatsCommand::Usage() + "\n" +
         "\n" +
         "  filter  Print the samples matching a location and depth range.\n" +
         "          " + FilterCommand::Usage() + "\n" +
         "\n" +
         "  rank    Print the top N samples ranked by api_gravity or sulfur_pct.\n" +
         "          " + RankCommand::Usage() + "\n" +
         "\n" +
         "Run 'petrocli <command> --help' for command-specific usage.\n";
}

}  // namespace

std::unique_ptr<Command> ParseArgs(const std::vector<std::string>& args) {
  if (args.empty()) {
    throw std::runtime_error(
        "usage: petrocli <check|stats|filter|rank> --input <path> [options]\n"
        "Run 'petrocli --help' for details.");
  }

  if (IsHelpFlag(args[0])) {
    return std::make_unique<PrintCommand>(GlobalHelpText());
  }

  const auto& factories = CommandFactories();
  const auto it = factories.find(args[0]);
  if (it == factories.end()) {
    throw std::runtime_error("unknown command: '" + args[0] + "'\nRun 'petrocli --help' for details.");
  }

  return it->second(std::vector<std::string>(args.begin() + 1, args.end()));
}

}  // namespace petrocli
