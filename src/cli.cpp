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
      {"stats", &StatsCommand::Parse},
  };
  return kFactories;
}

}  // namespace

std::unique_ptr<Command> ParseArgs(const std::vector<std::string>& args) {
  if (args.empty()) {
    throw std::runtime_error("usage: petrocli <check|stats|filter> --input <path> [options]");
  }

  const auto& factories = CommandFactories();
  const auto it = factories.find(args[0]);
  if (it == factories.end()) {
    throw std::runtime_error("unknown command: '" + args[0] + "'");
  }

  return it->second(std::vector<std::string>(args.begin() + 1, args.end()));
}

}  // namespace petrocli
