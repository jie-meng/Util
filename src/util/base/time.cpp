#include "time.hpp"

namespace util
{

tm localTime()
{
    time_t timer;
    timer = ::time(0);
    tm* pn = localtime(&timer);
    return *pn;
}

tm gmtTime()
{
    time_t timer;
    timer = ::time(0);
    tm* pn = gmtime(&timer);
    return *pn;
}

time_t mkCurrentTime()
{
    tm t = localTime();
    return ::mktime(&t);
}

DateTime::DateTime(const tm& t) :
    year_(t.tm_year+1900),
    month_(t.tm_mon + 1),
    day_(t.tm_mday),
    hour_(t.tm_hour),
    minute_(t.tm_min),
    second_(t.tm_sec),
    weekday_(t.tm_wday)
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) :
    year_(year),
    month_(month),
    day_(day),
    hour_(hour),
    minute_(minute),
    second_(second)
{
    //calculate weekday here
}

} // namespace util
