#include "information.hpp"
#include "util/string.hpp"

namespace util
{

using namespace std;

Information::Information(const std::string& module, InformationLevel info_level, const std::string& info) :
    level_(info_level),
    module_(module),
    info_(info)
{
    time_ = localTime();
}

Information::~Information()
{}

std::string Information::getTimeStr() const
{
    return strFormat("%04d-%02d-%02d %02d:%02d:%02d",
                        getYear(),
                        getMonth(),
                        getDay(),
                        getHour(),
                        getMinute(),
                        getSecond());
}

std::string Information::getLevelStr() const
{
    switch (getLevel())
    {
    case verbose:
        return "verbose";
    case debug:
        return "debug";
    case info:
        return "info";
    case wanning:
        return "wanning";
    case error:
        return "error";
    case fatal:
        return "fatal";
    case assert:
        return "assert";
    case silent:
        return "silent";
    default:
        return "unknown_infomation_level";
    }
}

std::string Information::getWdayStr() const
{
    switch (getWday())
    {
    case 0:
        return "SUN";
    case 1:
        return "MON";
    case 2:
        return "TUE";
    case 3:
        return "WEN";
    case 4:
        return "THU";
    case 5:
        return "FRI";
    case 6:
        return "SAT";
    default:
        return "UNKNOWN_WDAY";
    }
}

std::string Information::getFormatedInformation() const
{
    return getTimeStr() + " [" + getModule() + "] " + getLevelStr() + " : " + getInfo();
}

} //namespace util
