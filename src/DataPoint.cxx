#include "DataPoint.h"
#include <sstream>

namespace project {

DataPoint::DataPoint(std::string date, int hour, double temperature, bool quality)
    : _date(date), _hour(hour), _temperature(temperature), _quality(quality) {
        _split_date();
    } 

// TODO: implement an input parameter check to make sure parameters are valid


std::string DataPoint::get_date() const {
    return _date;
};

int DataPoint::get_hour() const {
    return _hour;
};

double DataPoint::get_temperature() const {
    return _temperature;
};

bool DataPoint::good_quality() const {
    return _quality;
};


void DataPoint::_split_date() {
    std::stringstream ss(_date);
    std::string year, month, day;

    std::getline(ss, year, '-');
    std::getline(ss, month, '-');
    std::getline(ss, day, '-');
    _year = std::stoi(year);
    _month = std::stoi(month);
    _day = std::stoi(day);

};

int DataPoint::get_day() const {
    return _day;
};

int DataPoint::get_month() const {
    return _month;
};

int DataPoint::get_year() const {
    return _year;
};


} // namespace project

