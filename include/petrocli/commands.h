#pragma once

#include "petrocli/cli.h"

namespace petrocli {

/** Runs the command selected by `options`; returns the exit code. */
// TODO: implement command dispatch.
int RunCommand(const CliOptions& options);

}  // namespace petrocli
