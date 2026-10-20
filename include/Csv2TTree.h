#pragma once


#ifndef CSV_2_TTREE_H
#define CSV_2_TTREE_H

#include <string>
#include <vector>

namespace project {

    std::vector<DataPoint> parse_csv_file(std::string csvfile_path);
    
    // writes data from csv file into ROOT Ttree structure and saves as .root file
    void csv_to_root(std::string csvfile_path, std::string output_filename);


} // namespace project


// note: separated parse_csv_file and csv_to_root into two different functions in case
// we want to clean up the data in-between at a later stage of code development
#endif /*CSV_2_TTREE*/