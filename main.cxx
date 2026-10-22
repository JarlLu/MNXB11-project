// Used to test code when working on defining classes and methods

#include "DataPoint.h"
#include "Csv2TTree.h"
#include "Dat2TTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>

using namespace project;
  int main() {

    project::csv_to_root("datasets/testdata2.csv", "datasets/rootfiles/testroot2.root" );
    
    TFile file("./datasets/rootfiles/testroot2.root", "READ");
    file.ls();

    TTree* tree = nullptr;
    file.GetObject("temperature", tree);

    if (tree) {
        tree->Print();
    }


    project::dat_to_root_TTree("./datasets/uppsala_tm_1722-2022.dat","./datasets/rootfiles/uppsala_1722-2022.root");
    std::cout << "opened and closed succesfully \n";
      return 0;

    
    }