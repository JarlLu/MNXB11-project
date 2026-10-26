// Used to test code when working on defining classes and methods

#include "DataPoint.h"
#include "Csv2TTree.h"
#include "Dat2TTree.h"
#include "allcities.h"
#include "monthly_coldest.h"
#include "climate_change.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>

using namespace project;
  int main() {

    project::csv_to_root("datasets/testdata/testdata3.csv", "datasets/rootfiles/testroot3.root" );
    
    TFile file("./datasets/rootfiles/testroot3.root", "READ");
    file.ls();

    TTree* tree = nullptr;
    file.GetObject("data", tree);

    if (tree) {
        tree->Print();
    }

    std::cout << "\n*****************************\n";
    std::cout << "Climate change analysis starting\n";
    project::dat_to_root_TTree("./datasets/uppsala_tm_1722-2022.dat","./datasets/rootfiles/uppsala_1722-2022.root");
    project::climate_change("./datasets/rootfiles/uppsala_1722-2022.root");
    std::cout << "Climate change analysis finished\n";
    std::cout << "\n*****************************\n";

    DataPoint point = DataPoint("1902-02-29", 21, 21.1, true);

    std::cout << "Year: " << point.get_year() << "\n";
    std::cout << "Month: " << point.get_month() << "\n";
    std::cout << "Day: " << point.get_day() << "\n";

    
    // Monthly_coldest
std::cout << "\n*****************************\n";
std::cout << "Starting monthly coldest analysis..." << std::endl;

const char* cities[] = {
    "LUND",
    "UPPSALA",
    "KARLSTAD",
    "HALMSTAD"
};

for (const char* city : cities) {
    std::string csv_file =
        "./datasets/data_" + std::string(city) + "2020-2025.csv";

    std::string root_file =
        "./datasets/rootfiles/data_" + std::string(city) + "2020-2025.root";

    std::cout << "\nProcessing " << city << "..." << std::endl;

    project::csv_to_root(csv_file, root_file);
    project::monthly_coldest(root_file);
}

std::cout << "\nMonthly coldest analysis has finished!" << std::endl;

allcities();

std::cout << "\nFinished making a 2x2 figure!" << std::endl;

std::cout << "\n*****************************\n";
    return 0; 
    
    }