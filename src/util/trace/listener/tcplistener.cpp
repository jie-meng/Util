#include "tcplistener.hpp"

namespace util
{

using namespace std;

const int IPPROTO_TCP = 6;
const int TCP_NODELAY = 0x0001;

TcpListener::TcpListener() :
    InformationListener(),
    ss_(Family_IPv4)
{
    //ctor
}

TcpListener::~TcpListener()
{
    //dtor
    ss_.closeSocket();
}

void TcpListener::process(Information* pinfo)
{
    std::string str = pinfo->getFormatedInformation() + info_append_;
    ss_.clientSend(str.c_str(), str.length());
}

bool TcpListener::init(const std::string& ip, unsigned short port, const std::string& info_append)
{
    if (!ss_.init())
        return false;

    int opt = 1;
    ss_.setSockopt(IPPROTO_TCP, TCP_NODELAY, (const void*)&opt, sizeof(opt));

    if (!ss_.connect(ip, port))
        return false;

    info_append_ = info_append;
    return true;
}

void TcpListener::close()
{
    ss_.closeSocket();
}

} // util
