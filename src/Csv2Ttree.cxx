#include "DataPoint.h"
#include "Csv2Ttree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
    
    
namespace project {
   
    void parse_csv_file(std::string csvfile_path) {
   
    std::ifstream file(csvfile_path);

    if (!file.is_open()) {
      std::cerr << "Could not open the file! \n";
    };

    std::string line;

    //std::getline(file, line);

    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::cout << "The line: " << ss.str() << "\n";  
      std::string date;
      std::string time;
      std::string temperature_str;
      std::string quality_str;
      
      std::getline(ss, date, ';');
      std::getline(ss, time, ';');
      std::getline(ss, temperature_str, ';');
      std::getline(ss, quality_str, ';');
      
      double temperature = std::stod(temperature_str); // convert to double
      bool quality = (quality_str == "G"); // discards all data points that are not Green

      std::cout << date << " "
                << time << " "
                << temperature << " "
                << std::boolalpha << quality << "\n";
    };
    };
   


} // namespace project