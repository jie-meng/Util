#include "time.hpp"
#include "constants.hpp"

namespace util
{

DateTime DateTime::now()
{
    return DateTime(time(0));
}

double DateTime::drift(const DateTime& dt1, const DateTime& dt0)
{
    return difftime(dt1.timet_, dt0.timet_);
}

DateTime::DateTime() :
    timet_(time(0))
{
}

DateTime::DateTime(time_t timet) :
    timet_(timet)
{
}

DateTime::DateTime(const tm& t)
{
    tm* ptm = const_cast<tm*>(&t);
    timet_ = mktime(ptm);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
{
    tm t;
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour =  hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = 0;
    
    timet_ = mktime(&t);
}

int DateTime::getYear() const
{ 
    return timet2tm()->tm_year + 1900;
}

int DateTime::getMonth() const
{
    return timet2tm()->tm_mon + 1;
}

int DateTime::getDay() const
{ 
    return timet2tm()->tm_mday;
}

int DateTime::getHour() const
{ 
    return timet2tm()->tm_hour;
}

int DateTime::getMinute() const
{ 
    return timet2tm()->tm_min;
}

int DateTime::getSecond() const
{ 
    return timet2tm()->tm_sec;
}

int DateTime::getWeekday() const
{ 
    return timet2tm()->tm_wday;
}

std::string DateTime::format(const std::string& fmt) const
{
    if (fmt.empty())
    {
        return std::string(ctime(&timet_));
    }
    
    char buf[kBufSize];
    strftime(buf, kBufSize, fmt.c_str(), timet2tm());
    
    return std::string(buf);
}

tm* DateTime::timet2tm() const
{
    return localtime(&timet_);
}

} // namespace util
