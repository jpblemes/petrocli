#include "petrocli/stats.h"

#include <algorithm>
#include <numeric>

namespace petrocli {

DescriptiveStats ComputeDescriptiveStats(const std::vector<double>& values) {
  DescriptiveStats stats;
  stats.count = values.size();
  if (values.empty()) {
    return stats;
  }

  const auto [min_it, max_it] = std::minmax_element(values.begin(), values.end());
  const double sum = std::accumulate(values.begin(), values.end(), 0.0);

  stats.minimum = *min_it;
  stats.maximum = *max_it;
  stats.mean = sum / static_cast<double>(stats.count);
  return stats;
}

}  // namespace petrocli
