#ifndef UTIL_TIME_HPP
#define UTIL_TIME_HPP

#include <ctime>
#include <string>

namespace util
{

class DateTime
{
public:
    DateTime();
    explicit DateTime(time_t timet);
    explicit DateTime(const tm& t);
    DateTime(int year, int month, int day, int hour, int minute, int second);

    static DateTime now();
    static double drift(const DateTime& dt1, const DateTime& dt0);

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    int getWeekday() const;
    
    std::string format(const std::string& fmt) const;
private:
    tm* timet2tm() const;
private:
    time_t timet_;
};

} // namespace util

#endif // UTIL_TIME_HPP
