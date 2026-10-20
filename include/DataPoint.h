#pragma once


#ifndef DATA_POINT_H
#define DATA_POINT_H


#include <string>

namespace project {
class DataPoint {
    public:
        DataPoint(std::string date, int hour, double temperature, bool quality);
        
        std::string get_date();    
        int get_hour();
        double get_temperature();
        bool good_quality();

    private:
        std::string _date; // the date of measurement
        int _hour; // hour of measurement
        double _temperature; // temperature
        bool _quality; // G or not G

};

} // namespace project
#endif /*DATA_POINT_H*/