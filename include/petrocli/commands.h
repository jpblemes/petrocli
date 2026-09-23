#pragma once

#include <ostream>

#include "petrocli/cli.h"

namespace petrocli {

/** Runs the command selected by `options`, writing output to `out`;
 *  returns the exit code. Throws std::runtime_error on failure. */
int RunCommand(const CliOptions& options, std::ostream& out);

}  // namespace petrocli
