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

DateTime::DateTime(tm* ptm) :
    year_(ptm->tm_year+1900),
    month_(ptm->tm_mon + 1),
    day_(ptm->tm_mday),
    hour_(ptm->tm_hour),
    minute_(ptm->tm_min),
    second_(ptm->tm_sec),
    weekday_(ptm->tm_wday)
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
