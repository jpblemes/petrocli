#pragma once

#include <cstddef>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Returns up to `limit` samples ranked by api_gravity, highest
 *  first; ties broken by sample_id, ascending. */
std::vector<Sample> RankByApiGravity(const std::vector<Sample>& samples, std::size_t limit);

/** Returns up to `limit` samples ranked by sulfur_pct, lowest
 *  first; ties broken by sample_id, ascending. */
std::vector<Sample> RankBySulfurPct(const std::vector<Sample>& samples, std::size_t limit);

}  // namespace petrocli
