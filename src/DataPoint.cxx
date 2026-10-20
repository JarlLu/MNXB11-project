#include "DataPoint.h"

namespace project {

DataPoint::DataPoint(std::string date, int hour, double temperature, bool quality)
    : _date(date), _hour(hour), _temperature(temperature), _quality(quality) {} 

// TODO: implement an input parameter check to make sure parameters are valid


std::string DataPoint::get_date() {
    return _date;
};

int DataPoint::get_hour() {
    return _hour;
};

double DataPoint::get_temperature() {
    return _temperature;
};

bool DataPoint::good_quality() {
    return _quality;
};

} // namespace project

