// Used to test code when working on defining classes and methods

#include "DataPoint.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace project;
int main() {
    project::DataPoint datapoint = project::DataPoint("1880-01-01", 20, 20.3, true);
    std::cout << "Date: " << datapoint.get_date() << "\n";
    std::cout << "Temp: " << datapoint.get_temperature() << "\n";


    std::ifstream file("datasets/testdata.csv");

    if (!file.is_open()) {
      std::cerr << "Could not open the file! \n";
      return 1;
    }

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
    }
    return 0;
  }