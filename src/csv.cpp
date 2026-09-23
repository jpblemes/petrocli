#include "petrocli/csv.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace petrocli {

namespace {

// Removes a trailing '\r' so CRLF-terminated lines parse like LF ones.
void StripTrailingCr(std::string& line) {
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }
}

// Throws if `field` isn't entirely a valid number.
double ParseDouble(const std::string& field, const std::string& column_name) {
  std::size_t chars_consumed = 0;
  double value = 0.0;
  try {
    value = std::stod(field, &chars_consumed);
  } catch (const std::exception&) {
    throw std::runtime_error("invalid " + column_name + " value: '" + field + "'");
  }
  if (chars_consumed != field.size() || !std::isfinite(value)) {
    throw std::runtime_error("invalid " + column_name + " value: '" + field + "'");
  }
  return value;
}

}  // namespace

std::vector<std::string> SplitCsvLine(const std::string& line) {
  std::vector<std::string> fields;
  std::size_t start = 0;
  while (true) {
    const std::size_t comma = line.find(',', start);
    if (comma == std::string::npos) {
      fields.push_back(line.substr(start));
      break;
    }
    fields.push_back(line.substr(start, comma - start));
    start = comma + 1;
  }
  return fields;
}

bool IsValidHeader(const std::vector<std::string>& fields) {
  static const std::vector<std::string> kExpectedHeader = {
      "sample_id",  "depth_m",       "api_gravity",
      "sulfur_pct", "density_g_cm3", "location"};
  return fields == kExpectedHeader;
}

Sample ParseSampleFields(const std::vector<std::string>& fields) {
  constexpr std::size_t kFieldCount = 6;
  if (fields.size() != kFieldCount) {
    throw std::runtime_error("expected 6 fields, got " + std::to_string(fields.size()));
  }

  Sample sample;
  sample.sample_id = fields[0];
  sample.depth_m = ParseDouble(fields[1], "depth_m");
  sample.api_gravity = ParseDouble(fields[2], "api_gravity");
  sample.sulfur_pct = ParseDouble(fields[3], "sulfur_pct");
  sample.density_g_cm3 = ParseDouble(fields[4], "density_g_cm3");
  sample.location = fields[5];
  return sample;
}

std::vector<Sample> ReadCsv(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("could not open CSV file: " + path);
  }

  std::string line;

  if (!std::getline(file, line)) {
    throw std::runtime_error(path + ": empty file, expected a header row");
  }
  StripTrailingCr(line);

  if (!IsValidHeader(SplitCsvLine(line))) {
    throw std::runtime_error(
        path +
        ":1: invalid header, expected "
        "sample_id,depth_m,api_gravity,sulfur_pct,density_g_cm3,location");
  }

  std::vector<Sample> samples;
  std::size_t line_number = 1;

  while (std::getline(file, line)) {
    ++line_number;
    StripTrailingCr(line);

    if (line.empty()) {
      continue;
    }

    try {
      samples.push_back(ParseSampleFields(SplitCsvLine(line)));
    } catch (const std::exception& e) {
      throw std::runtime_error(
          path + ":" + std::to_string(line_number) + ": " + e.what());
    }
  }

  return samples;
}

}  // namespace petrocli
