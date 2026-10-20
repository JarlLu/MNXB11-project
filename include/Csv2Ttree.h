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
#endif /*CSV_2_TTREE*/