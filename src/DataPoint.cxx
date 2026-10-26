#include "DataPoint.h"
#include <sstream>
// could not use chrono due to using c++17 and chrono was added in c++20 (I think?)
#include <date/include/date/date.h>
ClassImp(project::DataPoint);
namespace project {



DataPoint::DataPoint() : _date(""), _hour(-1),_day(0), _month(0),_year(0), _temperature(0.0), _quality(false) {}

DataPoint::DataPoint(std::string date, int hour, double temperature, bool quality)
    : _date(date), _hour(hour), _temperature(temperature), _quality(quality) {
        _split_date();
    } 

DataPoint::DataPoint(std::string date, double temperature) : _date(date), _hour(-1), _temperature(temperature), _quality(true) {_split_date();}

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

void DataPoint::set_values(std::string date, int hour, double temperature, bool quality) {
    _date=date;
    _hour=hour;
    _temperature=temperature;
    _quality=quality;
    _split_date();
}

void DataPoint::_split_date() {
    std::stringstream ss(_date);
    std::string year, month, day;

    std::getline(ss, year, '-');
    std::getline(ss, month, '-');
    std::getline(ss, day, '-');
    _year = std::stoi(year);
    _month = std::stoi(month);
    _day = std::stoi(day);

    _set_day_of_week();

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

int DataPoint::get_day_of_week() const {
    return _day_of_week;
}

void DataPoint::_set_day_of_week() {

    date::year_month_day date{
        date::year{_year},
        date::month{static_cast<unsigned>(_month)}, // std::chrono expects unsigned int values, hence static_cast
        date::day{static_cast<unsigned>(_day)}
    };

    // set value -1 if date not valid
    if (!date.ok()) { 
        _day_of_week = -1;
        return;
    }

    date::weekday wd{date::sys_days{date}};
    _day_of_week = wd.iso_encoding();
}

} // namespace project

