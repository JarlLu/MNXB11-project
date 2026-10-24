#include "climate_change.h"
#include "DataPoint.h"


#include <string>

#include <TFile.h>
#include <TTree.h>

namespace project {
    void climate_change(std::string root_file_path) {
    TFile *root_file=TFile::Open(root_file_path.c_str(), "READ"); //open file
    
    if (root_file==nullptr || root_file->IsZombie()) { //Making sure file opened properly
        std::cerr << "Error opening root file \n";
        return;
    }
    TTree *tree = root_file->Get("data"); //get tree with datapoints from file

    DataPoint *datapoint=nullptr; //initialize datapoint object
    tree->SetBranchAddress("datapoint", &datapoint); //set branch address to the newly initialized datapoint object



    }
}