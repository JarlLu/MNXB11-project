// Used to test code when working on defining classes and methods

#include "DataPoint.h"
#include "Csv2TTree.h"
#include "Dat2TTree.h"
#include "monthly_coldest.h"
#include "climate_change.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include "HottestDay.h"

using namespace project;
  int main() {
    project::csv_to_root("datasets/smhi-opendata_1_52230_20261007_165001_Falsterbo.csv", "datasets/rootfiles/testrootFalsterbo.root" );
    
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

    //Monthly_coldest
    std::cout << "\n*****************************\n";
    std::cout << "Starting monthly coldest analysis..."<< std::endl;
    project::csv_to_root("./datasets/testdata/LUND2015-2025.csv", "./datasets/rootfiles/LUND2015-2025.root");
    project::monthly_coldest("./datasets/rootfiles/LUND2015-2025.root");
    std::cout << "Monthly coldest analysis has finished!" << std::endl;
    std::cout << "\n*****************************\n";
 

    project::plot_av_temp_weekday_comparison("datasets/smhi-opendata_1_53430_20261007_164857_Lund.csv",
                                              "av_temp_weekday_comparison", {1980, 2000, 2020});

    return 0; 
    
    }