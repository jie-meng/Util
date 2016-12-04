#ifndef UTIL_TIME_HPP
#define UTIL_TIME_HPP

#include <ctime>

namespace util
{

tm localTime();

tm gmtTime();

time_t mkCurrentTime();
    
class DateTime
{
public:
    explicit DateTime(const tm& t);
    DateTime(int year, int month, int day, int hour, int minute, int second);

    inline int getYear() { return year_; }
    inline int getMonth() { return month_; }
    inline int getDay() { return day_; }
    inline int getHour() { return hour_; }
    inline int getMinute() { return minute_; }
    inline int getSecond() { return second_; }
    inline int getWeekday() { return weekday_; }
private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;
    int weekday_;
};

} // namespace util

#endif // UTIL_TIME_HPP
