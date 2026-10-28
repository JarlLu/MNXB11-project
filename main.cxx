// Used to test code when working on defining classes and methods

#include <TFile.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Csv2TTree.h"
#include "Dat2TTree.h"
#include "DataPoint.h"
#include "Weekday.h"
#include "allcities.h"
#include "climate_change.h"
#include "monthly_coldest.h"

using namespace project;
int main() {

    std::cout << "\n*****************************\n";
    std::cout << "Climate change analysis starting\n";
    project::dat_to_root_TTree("./datasets/uppsala_tm_1722-2022.dat",
                               "./datasets/rootfiles/uppsala_1722-2022.root");
    project::climate_change("./datasets/rootfiles/uppsala_1722-2022.root");
    std::cout << "Climate change analysis finished\n";
    std::cout << "\n*****************************\n";

    // Monthly_coldest
    std::cout << "\n*****************************\n";
    std::cout << "Starting monthly coldest analysis..." << std::endl;

    // use array and pointers pointing to c string
    const char* cities[] = {"LUND", "UPPSALA", "KARLSTAD", "HALMSTAD"};

    for (const char* city : cities) {
        // loop though all city names
        std::string csv_file =
            "./datasets/data_" + std::string(city) + "2020-2025.csv";
        std::string root_file =
            "./datasets/rootfiles/data_" + std::string(city) + "2020-2025.root";

        std::cout << "\nProcessing " << city << "..." << std::endl;
        // take files as input
        project::csv_to_root(csv_file, root_file);
        project::monthly_coldest(root_file);
    }

    std::cout << "\nMonthly coldest analysis has finished!" << std::endl;
    // create the mega plot
    allcities();

    std::cout << "\nFinished making a 2x2 figure!" << std::endl;
    std::cout << "\n*************************************\n";

    // Weekday analysis
    std::cout << "\n*************************************\n";
    std::cout << "Starting Weekday analysis..." << std::endl;

    // setting parameters
    std::vector<int> years = {1975, 1990, 2005, 2015, 2025};
    std::string csv_file_path =
        "datasets/smhi-opendata_1_53430_20261007_164857_Lund.csv";
    std::string output_plot_name = "weekday_comparison_Lund";

    // calling function
    project::plot_av_temp_weekday_comparison(csv_file_path, output_plot_name,
                                             years);

    std::cout << "Weekday analysis is finished!" << std::endl;
    std::cout << "\n*************************************\n";

    return 0;
}
