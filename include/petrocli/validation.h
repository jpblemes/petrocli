#pragma once

#include <string>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** True if `fields` are the six expected columns, in order. */
bool IsValidHeader(const std::vector<std::string>& fields);

/** Throws std::runtime_error if `fields` doesn't have exactly six columns. */
void ValidateFieldCount(const std::vector<std::string>& fields);

/** Converts `field` to a finite double; throws std::runtime_error,
 *  naming `field_name`, if it isn't a clean, finite number. */
double ValidateNumericField(const std::string& field, const std::string& field_name);

/** Throws std::runtime_error if `sample` has a non-finite value, a
 *  negative or non-integer depth_m, or an empty location. */
void ValidateSample(const Sample& sample);

}  // namespace petrocli
