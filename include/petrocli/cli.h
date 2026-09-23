#pragma once

namespace petrocli {

/** Parsed command-line arguments. */
struct CliOptions {};

/** Parses command-line arguments into a CliOptions. */
// TODO: implement argument parsing.
CliOptions ParseArgs(int argc, char** argv);

}  // namespace petrocli
