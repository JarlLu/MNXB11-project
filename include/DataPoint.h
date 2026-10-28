#pragma once

#include <TObject.h>
#include <string>

namespace project {
/**
 * @brief Represents one data point
 *
 */
class DataPoint : public TObject {
  public:
    /**
     * @brief Construct a new Data Point object
     *
     */
    DataPoint();

    /**
     * @brief Construct a new Data Point object from a .csv file
     *
     * @param date
     * @param hour
     * @param temperature
     * @param quality
     */
    DataPoint(std::string date, int hour, double temperature, bool quality);

    /**
     * @brief Construct a new Data Point object using the Uppsala dataset
     *
     * @param date
     * @param temperature
     */
    DataPoint(std::string date, double temperature);

    /**
     * @brief Get the date
     *
     * @return std::string
     */
    std::string get_date() const;

    /**
     * @brief Get the hour the measurement was taken
     *
     * @return int
     */
    int get_hour() const;

    /**
     * @brief Get the temperature
     *
     * @return double
     */
    double get_temperature() const;

    /**
     * @brief Checks if measurement was deemed of good quality
     *
     * @return true (G)
     * @return false (Y/R)
     */
    bool good_quality() const;

    /**
     * @brief Get the day of the measurement
     *
     * @return int
     */
    int get_day() const;

    /**
     * @brief Get the month
     *
     * @return int
     */
    int get_month() const;

    /**
     * @brief Get the year
     *
     * @return int
     */
    int get_year() const;

    /**
     * @brief Get the day of week
     *
     * @return int
     */
    int get_day_of_week() const;

    /**
     * @brief Set datapoint info
     *
     * @param date
     * @param hour
     * @param temperature
     * @param quality
     */
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

    /**
     * @brief Private setter for day, year and month using info from _date
     *
     */
    void _split_date();

    /**
     * @brief Sets the weekday the measurement was taken
     *
     */
    void _set_day_of_week();

    /**
     * @brief class def.
     *
     */
    ClassDef(DataPoint, 1);
    // TODO: make quality include yellow results as well
};

} // namespace project
