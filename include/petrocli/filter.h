#pragma once

#include <string>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Returns the samples in `samples` whose location equals
 *  `location` and whose depth_m falls within [min_depth,
 *  max_depth], inclusive. */
std::vector<Sample> FilterSamples(const std::vector<Sample>& samples, const std::string& location,
                                   double min_depth, double max_depth);

}  // namespace petrocli
