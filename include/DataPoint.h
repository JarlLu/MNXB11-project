#ifndef DATA_POINT_H
#define DATA_POINT_H

#include <string>

class DataPoint() {
    public:
        DataPoint(std::string& date, int hour, double temperature, bool quality);
        

        bool good_quality() {return _quality};

    private:
        std::string& _date; // the date of measurement
        int _hour; // hour of measurement
        double _temperature; // temperature
        bool _quality; // G or not G

};

#endif /*DATA_POINT_H*/