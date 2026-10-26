#pragma once

#include "DataPoint.h"
#include <TTree.h>
#include <string>
#include <vector>

namespace project {
void extract_relevant_data(std::string csv_file_path,
                           std::string output_file_name);

std::vector<DataPoint> hottest_weekend_filter(DataPoint datapoint);

std::vector<DataPoint> year_filter(DataPoint datapoint, int year);

std::array<double, 7>
average_temp_by_weekday(std::vector<DataPoint> datapoints);

void plot_average_temp_by_weekday(std::string csv_file_path,
                                  std::string output_file_name, int year);
void plot_av_temp_weekday_comparison(std::string csv_file_path,
                                     std::string output_file_name,
                                     std::vector<int> years);
} // namespace project