#include "tracemgr.hpp"
#include <cstdio>
#include <cstdarg>
#include "informationthread.hpp"

namespace util
{

using namespace std;

struct TraceMgr::TraceMgrImpl
{
    TraceMgrImpl(bool listener_sync) :
        information_thread(listener_sync)
    {}

    bool start()
    {
        return information_thread.start();
    }

    void stop()
    {
        information_thread.stop();
    }

    void trace(const std::string& module,
               Information::InformationLevel level,
               const std::string& info)
    {
        information_thread.queueMsg(new Information(module, level, info));
    }

    bool addListener(InformationListener* listener)
    {
        return information_thread.addListener(listener);
    }

    bool removeListener(InformationListener* listener)
    {
        return information_thread.removeListener(listener);
    }

    void clearAllListeners()
    {
        information_thread.clearAllListeners();
    }

    //fields
    InformationThread information_thread;
};

TraceMgr::TraceMgr(bool listener_sync) :
    impl_(new TraceMgrImpl(listener_sync))
{}

TraceMgr::~TraceMgr()
{}

bool TraceMgr::start()
{ return impl_->start(); }

void TraceMgr::trace(const std::string& module,
                     Information::InformationLevel level,
                     const std::string& info)
{ impl_->trace(module, level, info); }

bool TraceMgr::addListener(InformationListener* listener)
{
    return impl_->addListener(listener);
}

bool TraceMgr::removeListener(InformationListener* listener)
{
    return impl_->removeListener(listener);
}

void TraceMgr::clearAllListeners()
{
    impl_->clearAllListeners();
}

void TraceMgr::trace(const std::string& module,
               Information::InformationLevel level,
               const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[util::kBufSize] = {0};
    vsnprintf(buffer, util::kBufSize, fmt, ap);
    va_end(ap);

    impl_->trace(module, level, std::string(buffer));
}

void TraceMgr::stop()
{
    impl_->stop();
}

} //namespace util
