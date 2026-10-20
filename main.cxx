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

    project::csv_to_root("datasets/testdata2.csv", "datasets/rootfiles/testroot2.root" );
    
    TFile file("/home/hannap/mnxb11/MNXB11-project/datasets/rootfiles/testroot2.root", "READ");
    file.ls();

    TTree* tree = nullptr;
    file.GetObject("temperature", tree);

    if (tree) {
        tree->Print();
    }

      return 0;
    }