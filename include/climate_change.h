#pragma once

#include <TTree.h>
#include <string>
#include <vector>

namespace project {
// Opens root file and generates plot in images folder
void climate_change(std::string root_file_path);
// takes TTree with datapoints and specified year and returns the average
// temperature
double average_temperature(TTree* tree, int year);
// Plots the average temperature for each year in the dataset and 10/20 year
// moving averages with deviation from mean
void plot_average_temperature(TTree* tree, int first_year, int last_year,
                              std::string output_file_path);
// Calculates the moving average between year-window_size/2 and
// year+window_size/2 using the average annual temperature values in a vector
double moving_average(const std::vector<double>& average_temperatures, int year,
                      int window_size, int first_year, int last_year);
} // namespace project