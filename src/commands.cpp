#include "petrocli/commands.h"

#include <cstddef>
#include <iomanip>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "petrocli/csv.h"
#include "petrocli/stats.h"

namespace petrocli {

namespace {

// Parses a single `--input <path>` flag; throws `usage` on any
// other flag or a missing/empty path.
std::string ParseInputPathArg(const std::vector<std::string>& args, const std::string& usage) {
  std::string input_path;

  for (std::size_t i = 0; i < args.size(); ++i) {
    if (args[i] != "--input") {
      throw std::runtime_error("unknown argument: '" + args[i] + "'");
    }
    if (i + 1 >= args.size()) {
      throw std::runtime_error("--input requires a path argument");
    }
    input_path = args[++i];
  }

  if (input_path.empty()) {
    throw std::runtime_error(usage);
  }

  return input_path;
}

std::vector<double> ExtractColumn(const std::vector<Sample>& samples, double Sample::*field) {
  std::vector<double> values;
  values.reserve(samples.size());
  for (const Sample& sample : samples) {
    values.push_back(sample.*field);
  }
  return values;
}

constexpr int kColumnWidth = 15;
constexpr int kFieldWidth = 9;

void PrintStatsHeader(std::ostream& out) {
  out << std::left << std::setw(kColumnWidth) << "Column" << std::right << std::setw(kFieldWidth)
      << "Count" << std::setw(kFieldWidth) << "Min" << std::setw(kFieldWidth) << "Max"
      << std::setw(kFieldWidth) << "Mean" << "\n";
}

void PrintStatsRow(std::ostream& out, const std::string& column, const DescriptiveStats& stats) {
  out << std::left << std::setw(kColumnWidth) << column << std::right << std::setw(kFieldWidth)
      << stats.count;
  if (stats.count == 0) {
    out << std::setw(kFieldWidth) << "-" << std::setw(kFieldWidth) << "-" << std::setw(kFieldWidth)
        << "-" << "\n";
    return;
  }
  out << std::fixed << std::setprecision(2) << std::setw(kFieldWidth) << *stats.minimum
      << std::setw(kFieldWidth) << *stats.maximum << std::setw(kFieldWidth) << *stats.mean << "\n";
}

}  // namespace

std::unique_ptr<Command> CheckCommand::Parse(const std::vector<std::string>& args) {
  return std::make_unique<CheckCommand>(ParseInputPathArg(args, "usage: petrocli check --input <path>"));
}

CheckCommand::CheckCommand(std::string input_path) : input_path_(std::move(input_path)) {}

int CheckCommand::Execute(std::ostream& out) const {
  const std::vector<Sample> samples = ReadCsv(input_path_);
  out << "valid: " << input_path_ << " (" << samples.size() << " samples)\n";
  return 0;
}

std::unique_ptr<Command> StatsCommand::Parse(const std::vector<std::string>& args) {
  return std::make_unique<StatsCommand>(ParseInputPathArg(args, "usage: petrocli stats --input <path>"));
}

StatsCommand::StatsCommand(std::string input_path) : input_path_(std::move(input_path)) {}

int StatsCommand::Execute(std::ostream& out) const {
  const std::vector<Sample> samples = ReadCsv(input_path_);

  PrintStatsHeader(out);
  PrintStatsRow(out, "depth_m", ComputeDescriptiveStats(ExtractColumn(samples, &Sample::depth_m)));
  PrintStatsRow(out, "api_gravity", ComputeDescriptiveStats(ExtractColumn(samples, &Sample::api_gravity)));
  PrintStatsRow(out, "sulfur_pct", ComputeDescriptiveStats(ExtractColumn(samples, &Sample::sulfur_pct)));
  PrintStatsRow(out, "density_g_cm3",
                ComputeDescriptiveStats(ExtractColumn(samples, &Sample::density_g_cm3)));
  return 0;
}

}  // namespace petrocli
