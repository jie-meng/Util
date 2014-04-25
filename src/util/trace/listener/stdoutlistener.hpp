#ifndef UTIL_TRACE_LISTENER_STDOUTLISTENER_HPP
#define UTIL_TRACE_LISTENER_STDOUTLISTENER_HPP

#include "../information.hpp"

namespace util
{

class StdoutListener : public InformationListener
{
public:
    StdoutListener();
    virtual ~StdoutListener();
    virtual void process(Information* pinfo);
};

} // util

#endif // UTIL_TRACE_LISTENER_STDOUTLISTENER_HPP
