#include "Dat2TTree.h"
#include "DataPoint.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>

void project::dat_to_root_TTree(std::string datfile_path,
                                std::string output_file_path) {
    std::ifstream data_file(datfile_path); // open file

    if (!data_file) { // Making sure file opened properly
        std::cerr << "Error opening data file \n";
        return;
    }

    TFile* root_file = new TFile(output_file_path.c_str(), "RECREATE");
    TTree* tree = new TTree("data", "Temperature data");

    // initalize and set branch location of the data variables
    // std::string date;
    // Double_t temperature;
    // Bool_t quality;
    // tree->Branch("date", &date);
    // tree->Branch("temperature", &temperature);
    // tree->Branch("quality", &quality);
    DataPoint datapoint{"0000-00-00", 0, 0.0, false};
    tree->Branch("datapoint", &datapoint);

    // loop through all entries
    std::string line;
    while (std::getline(data_file, line)) {
        std::istringstream ss(line); // create stringstream

        // initialize string variables for each column
        std::string year_str;
        std::string month_str;
        std::string day_str;
        std::string temperature_str;
        std::string corrected_temperature_str;
        std::string quality_str;

        // assign strings from stringstream to correct variable.
        ss >> year_str >> month_str >> day_str >> temperature_str >>
            corrected_temperature_str >>
            quality_str; // seperates the stringstream line into the columns

        // change from strings to correct types
        int year = std::stoi(year_str);
        int month = std::stoi(month_str);
        int day = std::stoi(day_str);
        double temperature = std::stod(corrected_temperature_str);
        // int quality=std::stoi(quality_str);

        // create dates in the same format as the csv files have
        std::ostringstream date_stream;
        date_stream << year << '-' << std::setfill('0') << std::setw(2) << month
                    << '-' << std::setfill('0') << std::setw(2) << day;

        std::string date = date_stream.str();

        datapoint.set_values(date, -1, temperature, true);
        // put branches in tree
        tree->Fill();

        //
    }

    data_file.close(); // close .dat file

    tree->AutoSave();   // save tree in .root file
    root_file->Close(); // close .root file
}
