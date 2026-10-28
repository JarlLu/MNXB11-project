#pragma once

#include "DataPoint.h"
#include <array>
#include <string>
#include <vector>

namespace project {

/**
 * @brief Filters out DataPoints for a given year.
 *
 * @param datapoints
 * @return std::array<double, 7>
 */
std::vector<DataPoint> year_filter(std::vector<DataPoint> datapoints, int year);

/**
 * @brief Filters out weekend (Saturday + Sunday) DataPoints
 *
 * @param datapoints
 * @param years
 * @return std::vector<DataPoint>
 */
std::vector<DataPoint> weekend_filter(std::vector<DataPoint> datapoints);

/**
 * @brief Calculates the average temperature for each day of the week.
 *
 * @param datapoints
 * @return std::array<double, 7>
 */
std::array<double, 7>
average_temp_by_weekday(std::vector<DataPoint> datapoints);

/**
 * @brief Calculates average temperature for each day of the week.
 *
 * @param csv_file_path
 * @param output_file_name
 * @param year
 */
void plot_average_temp_by_weekday(std::string csv_file_path,
                                  std::string output_file_name, int year);

/**
 * @brief Create histogram of average temperature for each day of the week
 * (multiple years)
 *
 * @param csv_file_path
 * @param output_file_name
 * @param years
 */
void plot_av_temp_weekday_comparison(std::string csv_file_path,
                                     std::string output_file_name,
                                     std::vector<int> years);
} // namespace project