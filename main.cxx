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


    project::dat_to_root_TTree("./datasets/uppsala_tm_1722-2022.dat","./datasets/rootfiles/uppsala_1722-2022.root");
    project::climate_change("./datasets/rootfiles/uppsala_1722-2022.root");
    std::cout << "\n*****************************\n";

    DataPoint point = DataPoint("1902-02-29", 21, 21.1, true);

    std::cout << "Year: " << point.get_year() << "\n";
    std::cout << "Month: " << point.get_month() << "\n";
    std::cout << "Day: " << point.get_day() << "\n";
     // return 0;

    //Monthly_coldest
    std::cout << "Starting monthly coldest analysis..." << std::endl;

    monthly_coldest();

          std::cout << "Monthly coldest analysis has finished!" << std::endl;

    return 0; 
    
    }