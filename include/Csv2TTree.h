#pragma once

#include "DataPoint.h"
#include <string>
#include <vector>

namespace project {

/**
 * @brief parse csv file
 *
 * @param csvfile_path
 * @return std::vector<DataPoint>
 */
std::vector<DataPoint> parse_csv_file(std::string csvfile_path);

/**
 * @brief writes data from csv file into ROOT Ttree structure and saves as .root
 file

 *
 * @param csvfile_path
 * @param output_filename
 */
void csv_to_root(std::string csvfile_path, std::string output_filename);

/**
 * @brief Writes vector of DataPoints to ROOT file
 *
 * @param datapoints
 * @param root_filename
 */
void to_root(std::vector<DataPoint> datapoints, std::string root_filename);

} // namespace project

// note: separated parse_csv_file and csv_to_root into two different functions
// in case we want to clean up the data in-between at a later stage of code
// development
