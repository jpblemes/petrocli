#pragma once

#include <string>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Reads CSV data from `path` and returns the parsed samples. Not
 *  yet implemented. */
std::vector<Sample> ReadCsv(const std::string& path);

}  // namespace petrocli
