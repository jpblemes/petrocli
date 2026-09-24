#pragma once

#include <ostream>

namespace petrocli {

/** A runnable CLI command. */
class Command {
 public:
  virtual ~Command() = default;

  /** Runs the command, writing output to `out`; returns the exit
   *  code. Throws std::runtime_error on failure. */
  virtual int Execute(std::ostream& out) const = 0;
};

}  // namespace petrocli
