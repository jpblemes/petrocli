#pragma once

#include <string>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Reads CSV data from `path` and returns the parsed records. Not
 *  yet implemented. */
std::vector<Record> ReadCsv(const std::string& path);

}  // namespace petrocli
