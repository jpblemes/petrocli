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

bool BySulfurPctAscThenSampleIdAsc(const Sample& a, const Sample& b) {
  if (a.sulfur_pct != b.sulfur_pct) {
    return a.sulfur_pct < b.sulfur_pct;
  }
  return a.sample_id < b.sample_id;
}

std::vector<Sample> RankedAndLimited(std::vector<Sample> samples,
                                      bool (*less)(const Sample&, const Sample&),
                                      std::size_t limit) {
  // stable_sort: std::sort isn't guaranteed stable, so two samples that
  // tie on both the ranked field and sample_id could otherwise order
  // differently between standard library implementations.
  std::stable_sort(samples.begin(), samples.end(), less);
  if (samples.size() > limit) {
    samples.resize(limit);
  }
  return samples;
}

}  // namespace

std::vector<Sample> RankByApiGravity(const std::vector<Sample>& samples, std::size_t limit) {
  return RankedAndLimited(samples, ByApiGravityDescThenSampleIdAsc, limit);
}

std::vector<Sample> RankBySulfurPct(const std::vector<Sample>& samples, std::size_t limit) {
  return RankedAndLimited(samples, BySulfurPctAscThenSampleIdAsc, limit);
}

}  // namespace petrocli
