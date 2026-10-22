#pragma once


#ifndef DATA_POINT_H
#define DATA_POINT_H


#include <string>
#include <vector>

namespace project {
class DataPoint {
    public:
        /// @brief Represents one data point
        /// @param date Day of measurement
        /// @param hour Hour of measurement 
        /// @param temperature Temperature
        /// @param quality If is data point valid
        DataPoint(std::string date, int hour, double temperature, bool quality);        
        // should we add destructor?
        std::string get_date();    
        int get_hour();
        double get_temperature();
        bool good_quality();
        
        int get_day();
        int get_month();
        int get_year();


    private:
        std::string _date; // the date of measurement
        int _hour; // hour of measurement
        int _day; // day of measurement (date)
        int _month; // month of measurement
        int _year; // year of measurement
        double _temperature; // temperature
        bool _quality; // G or not G

        void _split_date(); // private setter for day, year and month, uses info from _date
        
        // TODO: make quality include yellow results as well

};

} // namespace project
#endif /*DATA_POINT_H*/