#include "DataPoint.h"
#include "Csv2Ttree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
    
    
namespace project {
   
    std::vector<DataPoint> parse_csv_file(std::string csvfile_path) {
   
        std::ifstream file(csvfile_path);

        // Throws error message if file cannot be opened
        if (!file.is_open()) {
        std::cerr << "Could not open the file! \n";
        };

        std::string line;
        std::vector<DataPoint> datapoints;

        //std::getline(file, line);

        // Loop through all lines in csv file one by one
        while (std::getline(file, line)) {
            std::stringstream ss(line);  // "extracts" one line
            std::cout << "The line: " << ss.str() << "\n";   // prints line (sanity check)

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
            std::cout << date << " "
                    << hour << " "
                    << temperature << " "
                    << std::boolalpha << quality << "\n";
            

            // creating instance of DataPoint
            DataPoint datapoint = DataPoint(date, hour, temperature, quality);      

            datapoints.push_back(datapoint);
            std::cout << "Successfully added a new datapoint to the list! \n"; // sanity check      
        };
    return datapoints;
    };


} // namespace project