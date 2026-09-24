#pragma once

#include <cstddef>
#include <vector>

#include "petrocli/model.h"

namespace petrocli {

/** Returns up to `limit` samples ranked by api_gravity, highest
 *  first; ties broken by sample_id, ascending. */
std::vector<Sample> RankByApiGravity(const std::vector<Sample>& samples, std::size_t limit);

}  // namespace petrocli
