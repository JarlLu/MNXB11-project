#pragma once

#ifndef DATA_POINT_H
#define DATA_POINT_H

#include <TObject.h>
#include <string>
#include <vector>

namespace project {
class DataPoint : public TObject {
  public:
    /// @brief Represents one data point
    /// @param date Day of measurement
    /// @param hour Hour of measurement
    /// @param temperature Temperature
    /// @param quality If is data point valid
    DataPoint();
    DataPoint(std::string date, int hour, double temperature, bool quality);

    DataPoint(std::string date,
              double temperature); // Custom constructor for uppsala dataset
    // should we add destructor?
    std::string get_date() const;
    int get_hour() const;
    double get_temperature() const;
    bool good_quality() const;

    int get_day() const;
    int get_month() const;
    int get_year() const;
    int get_day_of_week() const;
    void set_values(std::string date, int hour, double temperature,
                    bool quality);

  private:
    std::string _date;   // the date of measurement
    int _hour;           // hour of measurement
    int _day;            // day of measurement (date)
    int _month;          // month of measurement
    int _year;           // year of measurement
    int _day_of_week;    // day of week (Monday(1)-Sunday(7)
    double _temperature; // temperature
    bool _quality;       // G or not G

    void _split_date(); // private setter for day, year and month, uses info
                        // from _date
    void _set_day_of_week();
    ClassDef(DataPoint, 1);
    // TODO: make quality include yellow results as well
};

} // namespace project
#endif /*DATA_POINT_H*/