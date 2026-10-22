// Used to test code when working on defining classes and methods

#include "DataPoint.h"
#include "Csv2TTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>

using namespace project;
  int main() {

    project::csv_to_root("datasets/testdata/testdata3.csv", "datasets/rootfiles/testroot3.root" );
    
    TFile file("/home/hannap/mnxb11/MNXB11-project/datasets/rootfiles/testroot3.root", "READ");
    file.ls();

    TTree* tree = nullptr;
    file.GetObject("temperature", tree);

    if (tree) {
        tree->Print();
    }

    std::cout << "\n*****************************\n";

    DataPoint point = DataPoint("1902-02-29", 21, 21.1, true);

    std::cout << "Year: " << point.get_year() << "\n";
    std::cout << "Month: " << point.get_month() << "\n";
    std::cout << "Day: " << point.get_day() << "\n";
      return 0;
    }