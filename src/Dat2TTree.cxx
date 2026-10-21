#include "DataPoint.h"
#include "Dat2TTree.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>




void project::dat_to_root_TTree(std::string datfile_path, std::string output_file_name) {
    std::ifstream data_file(datfile_path); //open file

    if (!data_file) { //Making sure file opened properly
        std::cerr << "Error opening data file \n";
        return;
    }

    data_file.close();
}

