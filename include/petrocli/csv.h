#pragma once

#include <string>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Splits a CSV line into fields on commas; no escaping. */
std::vector<std::string> SplitCsvLine(const std::string& line);

/** True if `fields` are the six expected columns, in order. */
bool IsValidHeader(const std::vector<std::string>& fields);

/** Parses one split row into a Sample; throws on bad count or value. */
Sample ParseSampleFields(const std::vector<std::string>& fields);

/** Reads samples from `path`; throws `path:line: reason` on failure. */
std::vector<Sample> ReadCsv(const std::string& path);

}  // namespace petrocli
