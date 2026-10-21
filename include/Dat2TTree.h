#pragma once
#include <string>
#include <vector>
namespace project {
    // Extracts data from input .dat file and writes it into Datapoints in TTree structure and saves in .root file
    void dat_to_root_TTree(std::string datfile_path, std::string output_file_name);

}