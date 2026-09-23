#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "petrocli/cli.h"
#include "petrocli/commands.h"

int main(int argc, char** argv) {
  try {
    const std::vector<std::string> args(argv + 1, argv + argc);
    const petrocli::CliOptions options = petrocli::ParseArgs(args);
    return petrocli::RunCommand(options, std::cout);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
