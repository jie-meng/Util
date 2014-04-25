#include "udplistener.hpp"

namespace util
{

using namespace std;

UdpListener::UdpListener() :
    InformationListener(),
    ds_(Family_IPv4)
{
    //ctor
}

UdpListener::~UdpListener()
{
    //dtor
    ds_.closeSocket();
}

void UdpListener::process(Information* pinfo)
{
    std::string str = pinfo->getFormatedInformation() + info_append_;
    ds_.sendTo(str.c_str(), str.length(), ip_, port_);
}

bool UdpListener::init(const std::string& ip, unsigned short port, const std::string& info_append)
{
    if (!ds_.init())
        return false;

    ip_ = ip;
    port_ = port;
    info_append_ = info_append;

    return true;
}

void UdpListener::close()
{
    ds_.closeSocket();
}

} // util
