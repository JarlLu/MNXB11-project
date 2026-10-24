#pragma once

#include <string>
#include <TTree.h>

namespace project {
    void climate_change(std::string root_file_path);

    double average_temperature(TTree* tree, int year); //takes TTree with datapoints and specified year and returns the average temperature

    void plot_average_temperature(TTree* tree, int first_year, int last_year, std::string output_file_path);
}