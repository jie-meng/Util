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

} // namespace util
