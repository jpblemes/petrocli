#include "petrocli/filter.h"

#include <algorithm>
#include <iterator>

namespace petrocli {

std::vector<Sample> FilterSamples(const std::vector<Sample>& samples, const std::string& location,
                                   double min_depth, double max_depth) {
  std::vector<Sample> filtered;
  std::copy_if(samples.begin(), samples.end(), std::back_inserter(filtered),
               [&](const Sample& sample) {
                 return sample.location == location && sample.depth_m >= min_depth &&
                        sample.depth_m <= max_depth;
               });
  return filtered;
}

}  // namespace petrocli
