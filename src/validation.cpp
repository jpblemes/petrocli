#include "petrocli/validation.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace petrocli {

namespace {

// Throws if `value` isn't finite, naming `field_name`.
void RequireFinite(double value, const std::string& field_name) {
  if (!std::isfinite(value)) {
    throw std::runtime_error("invalid " + field_name + " value: not finite");
  }
}

}  // namespace

bool IsValidHeader(const std::vector<std::string>& fields) {
  static const std::vector<std::string> kExpectedHeader = {
      "sample_id",  "depth_m",       "api_gravity",
      "sulfur_pct", "density_g_cm3", "location"};
  return fields == kExpectedHeader;
}

void ValidateFieldCount(const std::vector<std::string>& fields) {
  constexpr std::size_t kFieldCount = 6;
  if (fields.size() != kFieldCount) {
    throw std::runtime_error("expected 6 fields, got " + std::to_string(fields.size()));
  }
}

double ValidateNumericField(const std::string& field, const std::string& field_name) {
  std::size_t chars_consumed = 0;
  double value = 0.0;
  try {
    value = std::stod(field, &chars_consumed);
  } catch (const std::exception&) {
    throw std::runtime_error("invalid " + field_name + " value: '" + field + "'");
  }
  if (chars_consumed != field.size() || !std::isfinite(value)) {
    throw std::runtime_error("invalid " + field_name + " value: '" + field + "'");
  }
  return value;
}

void ValidateSample(const Sample& sample) {
  RequireFinite(sample.depth_m, "depth_m");
  RequireFinite(sample.api_gravity, "api_gravity");
  RequireFinite(sample.sulfur_pct, "sulfur_pct");
  RequireFinite(sample.density_g_cm3, "density_g_cm3");

  if (sample.depth_m < 0.0 || sample.depth_m != std::trunc(sample.depth_m)) {
    throw std::runtime_error("invalid depth_m value: must be a non-negative integer");
  }
  if (sample.location.empty()) {
    throw std::runtime_error("invalid location value: must be non-empty");
  }
}

}  // namespace petrocli
