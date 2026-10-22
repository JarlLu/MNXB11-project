#include "DataPoint.h"
#include "Csv2TTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
    

#include <TString.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
    
namespace project {
   
    std::vector<DataPoint> parse_csv_file(std::string csvfile_path) {
   
        std::ifstream file(csvfile_path);

        // Throws error message if file cannot be opened
        if (!file.is_open()) {
        std::cerr << "Could not open the file! \n";
        };

        std::cout << "The file has been opened!\n";
        std::string line;
        std::vector<DataPoint> datapoints;

        //std::getline(file, line);

        // Loop through all lines in csv file one by one
        while (std::getline(file, line)) {
            std::stringstream ss(line);  // "extracts" one line
            //std::cout << "The line: " << ss.str() << "\n";   // prints line (sanity check)

            // prep for next step
            std::string date;
            std::string time;
            std::string hour_str;
            std::string temperature_str;
            std::string quality_str;
        
            // separate parameters (still as strings)
            std::getline(ss, date, ';');
            std::getline(ss, time, ';');
            std::getline(ss, temperature_str, ';');
            std::getline(ss, quality_str, ';');
            
            std::stringstream timestream(time);
            std::getline(timestream, hour_str,'-'); // extract only the hour from time string

            // convert to correct formats
            double temperature = std::stod(temperature_str); // convert to double
            bool quality = (quality_str == "G"); // discards all data points that are not Green
            int hour = std::stoi(hour_str); // xonvert to int

            // print parameter values (sanity check)
            //std::cout << date << " "
             //       << hour << " "
              //      << temperature << " "
              //      << std::boolalpha << quality << "\n";
            

            // creating instance of DataPoint
            DataPoint datapoint = DataPoint(date, hour, temperature, quality);      

            datapoints.push_back(datapoint);
            //std::cout << "Successfully added a new datapoint to the list! \n"; // sanity check      
        };

        std::cout << "Successfully extracted the datapoints from the CSV file!\n";
    return datapoints;
    };


    void csv_to_root(std::string csvfile_path, std::string output_filename) {
        
        // get datapoints
        std::vector<DataPoint> datapoints =  parse_csv_file(csvfile_path);


        // create the output root file
        TFile output(output_filename.c_str(),"RECREATE");        
        // RECREATE creates new file or overwrites existing ones -- warning!
        // .c_str converts std::string to the c-style string that ROOT expects

        // throw error if root file cannot be created
        if (output.IsZombie()) {
            std::cerr << "Could not create the ROOT file!\n";
            return;
        }

        std::cout << "Successfully created the ROOT file!\n";
        // create the Ttree
        TTree *tree = new TTree("data", "Temperature data");


        // prep parameters 
        std::vector<std::string> date;
        std::vector<Int_t> hour;
        std::vector<Double_t> temperature;
        std::vector<Bool_t> quality;

        // create branches
        tree->Branch("date", &date);
        tree->Branch("hour", &hour);
        tree->Branch("temperature", &temperature);
        tree->Branch("quality", &quality);

        std::cout << "Creating branches...\n";
        // iterate through datapoints

        for (project::DataPoint& dp : datapoints) {
            date.push_back(dp.get_date());
            hour.push_back(dp.get_hour());
            temperature.push_back(dp.get_temperature());
            quality.push_back(dp.good_quality());
            //std::cout << "Saved another datapoint!\n";
            };
        tree->Fill();
        std::cout << "Tree has been filled!\n";

        // cleaning up
        date.clear();
        hour.clear();
        temperature.clear();
        quality.clear();
        
        tree->AutoSave();
        std::cout << "The file has been saved!\n";
    };

    

} // namespace project