#ifndef UTIL_TRACE_TRACEMGR_HPP
#define UTIL_TRACE_TRACEMGR_HPP

#include "util/base.hpp"
#include "information.hpp"

namespace util
{

class TraceMgr
{
public:
    explicit TraceMgr(bool listener_sync = false);
    ~TraceMgr();
    bool start();
    void stop();
    void trace(const std::string& module,
               Information::InformationLevel level,
               const std::string& info);
    void trace(const std::string& module,
               Information::InformationLevel level,
               const char* fmt, ...);
    bool addListener(InformationListener* listener);
    bool removeListener(InformationListener* listener);
    void clearAllListeners();
private:
    class TraceMgrImpl;
    UtilAutoPtr<TraceMgrImpl> impl_;
private:
    DISALLOW_COPY_AND_ASSIGN(TraceMgr)
};

} //namespace util

#endif // UTIL_TRACE_TRACEMGR_HPP
