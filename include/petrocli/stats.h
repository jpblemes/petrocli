#pragma once

#include <cstddef>
#include <optional>
#include <vector>

namespace petrocli {

/** Count, minimum, maximum, and mean of a collection of values.
 *  minimum/maximum/mean are empty when count is zero. */
struct DescriptiveStats {
  std::size_t count = 0;
  std::optional<double> minimum;
  std::optional<double> maximum;
  std::optional<double> mean;
};

/** Computes DescriptiveStats for `values`; count is zero and the
 *  other fields stay empty when `values` is empty. */
DescriptiveStats ComputeDescriptiveStats(const std::vector<double>& values);

}  // namespace petrocli
