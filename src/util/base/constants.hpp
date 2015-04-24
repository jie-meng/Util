#ifndef UTIL_BASE_CONSTANTS_HPP
#define UTIL_BASE_CONSTANTS_HPP

#include <cstdlib>
#include <string>

namespace util
{

const size_t kBufSize = 4096;
const size_t kMaxPathLen = 512;
const int kSuccess = 1;
const int kFail = 0;
const std::string kPlatformUnix = "unix";
const std::string kPlatformWindows = "windows";

} // namespace util

#endif // UTIL_BASE_CONSTANTS_HPP
