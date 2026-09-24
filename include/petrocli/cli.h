#pragma once

#include <memory>
#include <string>
#include <vector>

#include "petrocli/command.h"

namespace petrocli {

/** Parses `args` (excluding the program name) into a Command;
 *  throws std::runtime_error on invalid usage. */
std::unique_ptr<Command> ParseArgs(const std::vector<std::string>& args);

}  // namespace petrocli
