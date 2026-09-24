#include "petrocli/rank.h"

#include <algorithm>

namespace petrocli {

namespace {

bool ByApiGravityDescThenSampleIdAsc(const Sample& a, const Sample& b) {
  if (a.api_gravity != b.api_gravity) {
    return a.api_gravity > b.api_gravity;
  }
  return a.sample_id < b.sample_id;
}

}  // namespace

std::vector<Sample> RankByApiGravity(const std::vector<Sample>& samples, std::size_t limit) {
  std::vector<Sample> ranked = samples;
  std::sort(ranked.begin(), ranked.end(), ByApiGravityDescThenSampleIdAsc);
  if (ranked.size() > limit) {
    ranked.resize(limit);
  }
  return ranked;
}

}  // namespace petrocli
