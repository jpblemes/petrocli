#pragma once

#include <string>

namespace petrocli {

/** One oil sample record, as read from the sample CSV. */
struct Sample {
  std::string sample_id;
  double depth_m;
  double api_gravity;
  double sulfur_pct;
  double density_g_cm3;
  std::string location;
};

}  // namespace petrocli
