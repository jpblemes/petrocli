#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "petrocli/cli.h"
#include "petrocli/command.h"

int main(int argc, char** argv) {
  try {
    const std::vector<std::string> args(argv + 1, argv + argc);
    const std::unique_ptr<petrocli::Command> command = petrocli::ParseArgs(args);
    return command->Execute(std::cout);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
