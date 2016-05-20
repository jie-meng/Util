#include "platform.hpp"
#include <vector>
#include "constants.hpp"
#include "string.hpp"

using namespace std;

namespace util
{

std::string platformInfo()
{
    vector<string> vec;
#ifdef _PLATFORM_UNIX_
    vec.push_back(kPlatformUnix);
#ifdef  __APPLE__
    vec.push_back(kPlatformMacOS);
#endif
#endif

#ifdef _PLATFORM_WINDOWS_
    vec.push_back(kPlatformWindows);
#endif
    return strJoin(vec, ",");
}

}
