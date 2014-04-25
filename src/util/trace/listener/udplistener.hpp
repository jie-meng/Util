#ifndef UTIL_TRACE_LISTENER_UDPLISTENER_HPP
#define UTIL_TRACE_LISTENER_UDPLISTENER_HPP

#include "util/net.hpp"
#include "../information.hpp"

namespace util
{

class UdpListener : public InformationListener
{
public:
    UdpListener();
    virtual ~UdpListener();
    virtual void process(Information* pinfo);

    bool init(const std::string& ip, unsigned short port, const std::string& info_append = "");
    void close();
private:
    util::DgramSocket ds_;
    std::string ip_;
    unsigned short port_;
    std::string info_append_;
private:
    DISALLOW_COPY_AND_ASSIGN(UdpListener)
};

} // util

#endif // UTIL_TRACE_LISTENER_UDPLISTENER_HPP
