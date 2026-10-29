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
    project::dat_to_root_TTree("datasets/uppsala_tm_1722-2022.dat",
                               "datasets/rootfiles/uppsala_1722-2022.root");
    project::climate_change("datasets/rootfiles/uppsala_1722-2022.root");
    std::cout << "Climate change analysis finished\n";
    std::cout << "\n*****************************\n";

    // Monthly_coldest
    std::cout << "\n*****************************\n";
    std::cout << "Starting monthly coldest analysis..." << std::endl;

    // use array and pointers pointing to c string
    const char* cities[] = {"LUND", "UPPSALA", "KARLSTAD", "HALMSTAD"};

for (const char* city : cities) {
    std::string city_name = city;

    std::string csv_file = "./datasets/data_" + city_name + "2020-2025.csv";

    // to_root() adds "datasets/" and ".root" automatically
    std::string root_file = "data_" + city_name + "2020-2025";

    std::cout << "\nProcessing " << city_name << "..." << std::endl;

    // Create datasets/rootfiles/data_CITY2020-2025.root
    project::csv_to_root(csv_file, root_file);

    // Open the actual file, including its directory and extension
    project::monthly_coldest("datasets/rootfiles/" + root_file + ".root");
}


    std::cout << "\nMonthly coldest analysis has finished!" << std::endl;

    //create the mega plot 
    allcities();

    std::cout << "\nFinished making a 2x2 figure!" << std::endl;
    std::cout << "\n*****************************\n";
    

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
    std::vector<int> years2;

    for (int year2 = 2006; year2 <= 2025; ++year2) {
        years2.push_back(year2);
    }

    project::plot_weekday_temp_trends(csv_file_path, "weekday_temp_trends",
                                      years2);

    std::cout << "Weekday analysis is finished!" << std::endl;
    std::cout << "\n*************************************\n";

    return 0;
}
