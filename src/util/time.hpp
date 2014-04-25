#ifndef UTIL_TIME_HPP
#define UTIL_TIME_HPP

#include "base.hpp"
#include <ctime>

namespace util
{

tm localTime();

tm gmtTime();

time_t mkCurrentTime();

} // namespace util

#endif // UTIL_TIME_HPP
