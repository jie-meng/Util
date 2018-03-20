#include "time.hpp"
#include "constants.hpp"
#include "string.hpp"
#include "baseclass.hpp"
#ifdef _PLATFORM_UNIX_
#include <sys/time.h>
#endif
#ifdef _PLATFORM_WINDOWS_
#include <windows.h>
#endif

namespace util
{

DateTime DateTime::now()
{
    return DateTime(time(0));
}

DateTime::DateTime() :
    timet_(time(0))
{}

DateTime::DateTime(time_t timet) :
    timet_(timet)
{}

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

DateTime::Weekday DateTime::getWeekday() const
{ 
    return (DateTime::Weekday)(timet2tm()->tm_wday+1);
}

int DateTime::getYearday() const
{
    return timet2tm()->tm_yday+1;
}

std::string DateTime::format(const std::string& fmt) const
{
    if (fmt.empty())
        return std::string(ctime(&timet_));

    char buf[kBufSize];
    strftime(buf, kBufSize, fmt.c_str(), timet2tm());

    return std::string(buf);
}

double DateTime::drift(const DateTime& dt) const
{
    return difftime(timet_, dt.timet_);
}

bool DateTime::equals(const DateTime& dateTime) const
{
    return timet_ == dateTime.timet_;
}

bool DateTime::earlierThan(const DateTime& dateTime) const
{
    return timet_ < dateTime.timet_;
}

tm* DateTime::timet2tm() const
{
    return localtime(&timet_);
}

std::string toString(const DateTime& dt) 
{ 
    return strTrim(dt.format("")); 
}

std::string toString(DateTime::Weekday weekday)
{
    switch (weekday)
    {
        case DateTime::Sunday:
            return "Sun";
        case DateTime::Monday:
            return "Mon";
        case DateTime::Tuesday:
            return "Tue";
        case DateTime::Wensday:
            return "Wen";
        case DateTime::Thursday:
            return "Thu";
        case DateTime::Friday:
            return "Fri";
        case DateTime::Saturday:
            return "Sat";
        default:
            throw Exception(strFormat("Invalid Weekday value: %d", (int)weekday));
    }
}

uint64_t getCurrentMiliseconds()
{
#ifdef _PLATFORM_UNIX_
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (uint64_t) tp.tv_sec * 1000L + tp.tv_usec / 1000;
#endif _PLATFORM_UNIX_

#ifdef _PLATFORM_WINDOWS_
    SYSTEMTIME time;
    GetSystemTime(&time);
    return (uint64_t)((time.wSecond * 1000) + time.wMilliseconds);
#endif
}

} // namespace util
