#include "petrocli/cli.h"
#include "petrocli/commands.h"

int main(int argc, char** argv) {
  const petrocli::CliOptions options = petrocli::ParseArgs(argc, argv);
  return petrocli::RunCommand(options);
}
