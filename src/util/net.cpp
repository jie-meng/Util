#include "net.hpp"
#include <vector>
#include <list>
#include "lexicalcast.hpp"
#include "regex.hpp"

#ifdef _PLATFORM_WINDOWS_
#ifndef _MSVC_
#define WINVER (0x0501)
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
#endif
#ifdef _PLATFORM_LINUX_
#ifdef _ANDROID_
#include <sys/select.h>
//#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>
#define closesocket close
#endif

namespace util
{

#define BIG_LITTLE_SWAP_16(A)     ((((uint16_t)(A) & 0xff00) >> 8) | \
                                  (((uint16_t)(A) & 0x00ff) << 8))

#define BIG_LITTLE_SWAP_32(A)     ((((uint32_t)(A) & 0xff000000) >> 24) | \
                                  (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                                  (((uint32_t)(A) & 0x0000ff00) << 8) | \
                                  (((uint32_t)(A) & 0x000000ff) << 24))

#define BIG_LITTLE_SWAP_64(A)     ((((uint64_t)(A) & 0xff00000000000000) >> 56) | \
                                  (((uint64_t)(A) & 0x00ff000000000000) >> 40) | \
                                  (((uint64_t)(A) & 0x0000ff0000000000) >> 24) | \
                                  (((uint64_t)(A) & 0x000000ff00000000) >> 8) | \
                                  (((uint64_t)(A) & 0x00000000ff000000) << 8) | \
                                  (((uint64_t)(A) & 0x0000000000ff0000) << 24) | \
                                  (((uint64_t)(A) & 0x000000000000ff00) << 40) | \
                                  (((uint64_t)(A) & 0x00000000000000ff) << 56))

uint16_t u_htons(uint16_t value)
{
    return BIG_LITTLE_SWAP_16(value);
}

uint16_t u_ntohs(uint16_t value)
{
    return BIG_LITTLE_SWAP_16(value);
}

uint32_t u_htonl(uint32_t value)
{
    return BIG_LITTLE_SWAP_32(value);
}

uint32_t u_ntohl(uint32_t value)
{
    return BIG_LITTLE_SWAP_32(value);
}

uint64_t u_htonll(uint64_t value)
{
    return BIG_LITTLE_SWAP_64(value);
}

uint64_t u_ntohll(uint64_t value)
{
    return BIG_LITTLE_SWAP_64(value);
}

std::string ipArrayToStr4(unsigned char* ip_byte_array, size_t len)
{
    if (!ip_byte_array || len < 4)
        return "";
    return util::strFormat("%d.%d.%d.%d",
                          ip_byte_array[0],
                          ip_byte_array[1],
                          ip_byte_array[2],
                          ip_byte_array[3]);
}

std::string ipArrayToStr6(unsigned char* ip_byte_array, size_t len)
{
    if (!ip_byte_array || len < 16)
        return "";

    std::list<std::string> ip_list;
    for (size_t i=0; i<8; ++i)
    {
        unsigned short data = u_ntohs(*(unsigned short*)&ip_byte_array[i*2]);
        if(0 == data)
            ip_list.push_back("$");
        else
            ip_list.push_back(util::strFormat("%x", data));
    }

    std::string str_format = util::strJoin(ip_list, ":");
    //'$' is '0' substatution
    if ("$:$:$:$:$:$:$:$" == str_format)
        return "::";
    else if (util::strContains(str_format, "$:$:$:$:$:$:$"))
        str_format = util::strReplace(str_format, "$:$:$:$:$:$:$", "#");
    else if (util::strContains(str_format, "$:$:$:$:$:$"))
        str_format = util::strReplace(str_format, "$:$:$:$:$:$", "#");
    else if (util::strContains(str_format, "$:$:$:$:$"))
        str_format = util::strReplace(str_format, "$:$:$:$:$", "#");
    else if (util::strContains(str_format, "$:$:$:$"))
        str_format = util::strReplace(str_format, "$:$:$:$", "#");
    else if (util::strContains(str_format, "$:$:$"))
        str_format = util::strReplace(str_format, "$:$:$", "#");
    else if (util::strContains(str_format, "$:$"))
        str_format = util::strReplace(str_format, "$:$", "#");
    else
        return util::strReplaceAll(str_format, "$", "0");

    str_format = util::strReplaceAll(str_format, "$", "0");
    if (util::strStartWith(str_format, "#") || util::strEndWith(str_format, "#"))
        return util::strReplace(str_format, "#", ":");
    else
        return util::strReplace(str_format, "#", "");
}

std::string ipArrayToStr(Family family, unsigned char* ip_byte_array, size_t len)
{
    switch(family)
    {
    case Family_IPv4:
        return ipArrayToStr4(ip_byte_array, len);
    case Family_IPv6:
        return ipArrayToStr6(ip_byte_array, len);
    default:
        break;
    }
    return "";
}

unsigned char* ipStrToArray4(const std::string& ip_str, unsigned char* ip_byte_array, size_t len)
{
    if (!ip_byte_array || len < 4)
        return 0;

    Regex regexer("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})");
    if(!regexer.match(ip_str))
        return 0;

    for (size_t i=0; i<4; ++i)
    {
        size_t data = lexicalCastDefault<size_t>(regexer.getMatchedGroup(i+1).c_str(), 256);
        if (data > 255)
        {
            memset(ip_byte_array, 0, 4);
            return 0;
        }
        ip_byte_array[i] = static_cast<unsigned char>(data);
    }
    return ip_byte_array;
}

unsigned char* ipStrToArray6(const std::string& ip_str, unsigned char* ip_byte_array, size_t len)
{
    if (!ip_byte_array || len < 16)
        return 0;

    if ("" == ip_str)
    {
        memset(ip_byte_array, 0, 16);
        return ip_byte_array;
    }

    if (util::strContains(ip_str, "::"))
    {
        //test only one "::"
        std::string ip_str_ex = util::strReplace(ip_str, "::", "#");
        if (util::strContains(ip_str_ex, "::"))
            return 0;

        //invalid format after delet "::"
        if (util::strStartWith(ip_str_ex, ":") || util::strEndWith(ip_str_ex, ":"))
            return 0;

        std::list<std::string> ip_list;
        size_t value_cnt = util::strSplit(ip_str_ex, ":", ip_list);
        if(0 == value_cnt)
            return 0;

        size_t zero_cnt = 8 - value_cnt;
        std::string zero_str;

        if (util::strStartWith(ip_str_ex, "#"))
        {
            for (size_t i=0; i<zero_cnt; ++i)
                zero_str += "0:";
        }
        else if (util::strEndWith(ip_str_ex, "#"))
        {
            for (size_t i=0; i<zero_cnt; ++i)
                zero_str += ":0";
        }
        else
        {
            //e.g. 2001#9 has 2 value, so value_cnt += 1
            value_cnt += 1;
            zero_cnt = 8 - value_cnt;

            for (size_t i=0; i<zero_cnt; ++i)
                zero_str += ":0";
            zero_str += ":";
        }
        //call it with formated ip_str
        return ipStrToArray6(util::strReplace(ip_str_ex, "#", zero_str), ip_byte_array, len);
    }
    else
    {
        std::vector<std::string> ipv6_str_vec;
        if(8 != util::strSplit(ip_str, ":", ipv6_str_vec))
            return 0;

        for (size_t i=0; i<ipv6_str_vec.size(); ++i)
        {
            if (!Regex("[0123456789aAbBcCdDeEfF]{1,4}").match(ipv6_str_vec[i]))
            {
                memset(ip_byte_array, 0, 16);
                return 0;
            }
            unsigned long data(0);
            if (!hexStrToDec(ipv6_str_vec[i].c_str(), data))
            {
                memset(ip_byte_array, 0, 16);
                return 0;
            }
            *(unsigned short*)&ip_byte_array[i*2] = u_htons(static_cast<unsigned short>(data));
        }
        return ip_byte_array;
    }

    return 0;
}

unsigned char* ipStrToArray(Family family, const std::string& ip_str, unsigned char* ip_byte_array, size_t len)
{
    switch(family)
    {
    case Family_IPv4:
        return ipStrToArray4(ip_str, ip_byte_array, len);
    case Family_IPv6:
        return ipStrToArray6(ip_str, ip_byte_array, len);
    default:
        break;
    }
    return 0;
}

bool isIpAddrEmpty(const TIpAddr& ip)
{
    size_t cmp_size = sizeof(ip.ipaddr);
    if (Family_IPv4 == ip.ipaddr_ver)
        cmp_size = 4;
    else if (Family_IPv6 == ip.ipaddr_ver)
        cmp_size = 16;

    for (size_t i=0; i<cmp_size; ++i)
    {
        if (0 != ip.ipaddr[i])
            return false;
    }
    return true;
}

bool isIpAddrEqual(const TIpAddr& ip_first, const TIpAddr& ip_second)
{
    if (ip_first.ipaddr_ver != ip_second.ipaddr_ver)
        return false;

    size_t cmp_size = sizeof(ip_first.ipaddr);
    if (Family_IPv4 == ip_first.ipaddr_ver)
        cmp_size = 4;
    else if (Family_IPv6 == ip_first.ipaddr_ver)
        cmp_size = 16;

    for (size_t i=0; i<cmp_size; ++i)
    {
        if (ip_first.ipaddr[i] != ip_second.ipaddr[i])
            return false;
    }
    return true;
}

//////////////////////////////////////////////////
// class name : SocketStartup
// description :
// author :
// time : 2013-01-08-21.27
//////////////////////////////////////////////////
struct SocketStartup::SocketStartupImpl
{
    SocketStartupImpl() :
        is_ok_(false)
    {
#ifdef _PLATFORM_WINDOWS_
        is_ok_ = win32Startup(2, 2) ? true : false;
#endif // _PLATFORM_WINDOWS_
    }

    ~SocketStartupImpl()
    {
#ifdef _PLATFORM_WINDOWS_
        win32Cleanup();
#endif // _PLATFORM_WINDOWS_
    }

#ifdef _PLATFORM_WINDOWS_
    bool win32Startup(unsigned char lobyte, unsigned char hibyte)
    {
        WORD wversion_requested;
        WSADATA wsa_data;
        wversion_requested = MAKEWORD(lobyte, hibyte);
        int err = WSAStartup(wversion_requested, &wsa_data);

        if (err != 0)
            return false;

        if(LOBYTE(wsa_data.wVersion) != lobyte ||
           HIBYTE(wsa_data.wVersion) != hibyte)
        {
            win32Cleanup();
            return false;
        }
        return true;
    }

    int win32Cleanup()
    {
        return WSACleanup();
    }
#endif // _PLATFORM_WINDOWS_

    bool is_ok_;
};

SocketStartup::SocketStartup() :
    pimpl_(new SocketStartupImpl())
{}

SocketStartup::~SocketStartup()
{}

bool SocketStartup::isOk() const
{
    return pimpl_->is_ok_;
}

int getFamilyCode(Family family)
{
    if (Family_IPv4 == family)
        return AF_INET;
    else if( Family_IPv6 == family)
        return AF_INET6;
    else
        return AF_UNSPEC;
}

int getSockTypeCode(SockType sock_type)
{
    if (SockType_Dgram == sock_type)
        return SOCK_DGRAM;
    else if (SockType_Stream == sock_type)
        return SOCK_STREAM;
    else
        return SOCK_RAW;
}

void get_TIpAddrInfo_from_sockaddr_storage(const struct sockaddr_storage& ss, TIpAddrInfo& out_tipaddrinfo)
{
    memset(&out_tipaddrinfo, 0, sizeof(out_tipaddrinfo));

    if(AF_INET == ss.ss_family || PF_INET == ss.ss_family)
    {
        out_tipaddrinfo.taddr.ipaddr_ver = Family_IPv4;
        sockaddr_in* ps4 = (sockaddr_in*)&ss;
        memcpy(out_tipaddrinfo.taddr.ipaddr, &ps4->sin_addr, 4);
        out_tipaddrinfo.port = u_ntohs(ps4->sin_port);
    }
    else
    {
        out_tipaddrinfo.taddr.ipaddr_ver = Family_IPv6;
        sockaddr_in6* ps6 = (sockaddr_in6*)&ss;
        memcpy(out_tipaddrinfo.taddr.ipaddr, &ps6->sin6_addr, 16);
        out_tipaddrinfo.port = u_ntohs(ps6->sin6_port);
    }
}

int createSocket(Family family, SockType sock_type, int protocol)
{
    return ::socket(getFamilyCode(family), getSockTypeCode(sock_type), protocol);
}

int closeSocket(int socket)
{
    return ::closesocket(socket);
}

struct timeval getTimeVal(size_t ms)
{
    timeval tv = {0};
    tv.tv_sec = ms / 1000;
    tv.tv_usec = 1000 * (ms % 1000);
    return tv;
};

//////////////////////////////////////////////////
// class name : Socket
// description :
// author :
// time : 2013-01-08-21.06
//////////////////////////////////////////////////
struct Socket::SocketData
{
    SocketData(Family family, SockType sock_type, int protocol) :
        socket_(kInvalidSocket),
        family_(family),
        sock_type_(sock_type),
        protocol_(protocol),
        paddrinfo_res0_(0)
    {}
    int socket_;
    Family family_;
    SockType sock_type_;
    int protocol_;
    addrinfo* paddrinfo_res0_;
    std::string addrinfo_ip_;
    std::string addrinfo_port_;
};

Socket::Socket(Family family, SockType sock_type, int protocol) :
    pdata_(new SocketData(family, sock_type, protocol))
{
    SocketStartup::getInstance();
    init();
}

bool Socket::init()
{
    if (kInvalidSocket == pdata_->socket_)
        pdata_->socket_ = createSocket(pdata_->family_, pdata_->sock_type_, pdata_->protocol_);
    return kInvalidSocket != pdata_->socket_;
}

bool Socket::isOk() const
{
    return kInvalidSocket != pdata_->socket_;
}

Socket::~Socket()
{
    if (pdata_->paddrinfo_res0_)
        ::freeaddrinfo(pdata_->paddrinfo_res0_);
    closeSocket();
}

int Socket::getSocket() const
{
    return pdata_->socket_;
}

//common
bool Socket::bind(const std::string& ip, unsigned short port, unsigned short* pport_out)
{
    if(!getAddrInfo(ip, port))
        return false;

    bool ret = (0 == ::bind(pdata_->socket_,
                  pdata_->paddrinfo_res0_->ai_addr,
                  pdata_->paddrinfo_res0_->ai_addrlen))
                  ? true : false;

    if (NULL != pport_out && ret)
    {
        if (Family_IPv4 == pdata_->family_)
        {
            sockaddr_in sin;
#ifdef _PLATFORM_WINDOWS_
             int len = sizeof(sin);
#endif
#ifdef _PLATFORM_LINUX_
            socklen_t len = sizeof(sin);
#endif
            if (getsockname(pdata_->socket_, (struct sockaddr *)&sin, &len) != 0)
                return ret;

            *pport_out = u_ntohs(sin.sin_port);
        }
        else if (Family_IPv6 == pdata_->family_)
        {
            sockaddr_in6 sin6;
#ifdef _PLATFORM_WINDOWS_
             int len = sizeof(sin6);
#endif
#ifdef _PLATFORM_LINUX_
            socklen_t len = sizeof(sin6);
#endif
            if (getsockname(pdata_->socket_, (struct sockaddr *)&sin6, &len) != 0)
                return ret;

            *pport_out = u_ntohs(sin6.sin6_port);
        }
    }

    return ret;
}

bool Socket::closeSocket()
{
    bool ret = (0 == ::closesocket(pdata_->socket_)) ? true : false;
    pdata_->socket_ = kInvalidSocket;
    return ret;
}

int Socket::setBlock(unsigned long block)
{
#ifdef _PLATFORM_WINDOWS_
    unsigned long ublock = block;
    return ::ioctlsocket(pdata_->socket_, FIONBIO, &ublock);
#endif
#ifdef _PLATFORM_LINUX_
    int flags  = fcntl(pdata_->socket_,F_GETFL,0);
    if (block)
        return ::fcntl(pdata_->socket_, F_SETFL, flags | O_NONBLOCK);   //set noblock
    else
        return ::fcntl(pdata_->socket_, F_SETFL, flags &~ O_NONBLOCK);    //set blockpp
#endif
}

int Socket::setSockopt(int level, int optname, const void* optval, int optlen)
{
    return setsockopt(pdata_->socket_, level, optname, (const char*)optval, optlen);
}

//dgram
int Socket::sendTo(const char* buf, size_t len, const std::string& ip, unsigned short port, int flag)
{
    if(!getAddrInfo(ip, port))
        return false;
    return ::sendto(pdata_->socket_,
                    buf,
                    len,
                    flag,
                    pdata_->paddrinfo_res0_->ai_addr,
                    pdata_->paddrinfo_res0_->ai_addrlen);
}

int Socket::recvFrom(char* buf, size_t len, std::string* pout_peer_ip, unsigned short* pout_peer_port, int flag)
{
    struct sockaddr_storage peer_addr = {0};
    socklen_t peer_addr_len = sizeof(peer_addr);

    memset(buf, 0, len);
    int ret = ::recvfrom(pdata_->socket_, buf, len, flag, (struct sockaddr*)&peer_addr, &peer_addr_len);
    if (peer_addr.ss_family == AF_INET6 || peer_addr.ss_family == PF_INET6)
    {
        //IPv6
        struct sockaddr_in6* p_addr6 = (struct sockaddr_in6*)&peer_addr;
        if (pout_peer_ip)
            *pout_peer_ip = ipArrayToStr(Family_IPv6, (unsigned char*)&p_addr6->sin6_addr, sizeof(p_addr6->sin6_addr));
        if (pout_peer_port)
            *pout_peer_port = u_ntohs(p_addr6->sin6_port);
    }
    else
    {
        //IPv4
        struct sockaddr_in* p_addr = (struct sockaddr_in*)&peer_addr;
        if (pout_peer_ip)
            *pout_peer_ip = ipArrayToStr(Family_IPv4, (unsigned char*)&p_addr->sin_addr, sizeof(p_addr->sin_addr));
        if (pout_peer_port)
            *pout_peer_port = u_ntohs(p_addr->sin_port);
    }

    return ret;
}

//stream
bool Socket::listen(size_t backlog)
{
    return (0 == ::listen(pdata_->socket_, backlog)) ? true : false;
}

int Socket::accept(struct TIpAddrInfo* pout_tipaddrinfo)
{
    struct sockaddr_storage addr_storage = {0};
    socklen_t addr_len(sizeof(addr_storage));

    int sock = ::accept(pdata_->socket_, (struct sockaddr*)&addr_storage, &addr_len);

    if (pout_tipaddrinfo)
        get_TIpAddrInfo_from_sockaddr_storage(addr_storage, *pout_tipaddrinfo);

    return sock;
}

bool Socket::connect(const std::string& ip,unsigned short port)
{
    if (!getAddrInfo(ip, port))
        return false;

    return (0 == ::connect(pdata_->socket_,
                           pdata_->paddrinfo_res0_->ai_addr,
                           pdata_->paddrinfo_res0_->ai_addrlen))
            ? true : false;
}

int Socket::send(int socket, const char* buf, size_t len, int flag)
{
    return ::send(socket, buf, len, flag);
}

int Socket::recv(int socket, char* buf, size_t len, int flag)
{
    memset(buf, 0, len);
    return ::recv(socket, buf, len, flag);
}

bool Socket::getAddrInfo(const std::string& ip, const std::string& port)
{
    if (pdata_->paddrinfo_res0_ && ip == pdata_->addrinfo_ip_ && port == pdata_->addrinfo_port_)
        return true;

    struct addrinfo hints = {0};
    hints.ai_family = getFamilyCode(pdata_->family_);
    hints.ai_socktype = getSockTypeCode(pdata_->sock_type_);
    hints.ai_protocol = pdata_->protocol_;
    hints.ai_flags = ("" != ip) ? AI_NUMERICHOST : AI_PASSIVE;

    if (pdata_->paddrinfo_res0_)
        ::freeaddrinfo(pdata_->paddrinfo_res0_);
    pdata_->paddrinfo_res0_ = 0;

    const char* ip_ptr = 0;
    const char* port_ptr = 0;
    if ("" != ip)
        ip_ptr = ip.c_str();
    if ("" != port)
        port_ptr = port.c_str();

    if (0 == ::getaddrinfo(ip_ptr, port_ptr, &hints, &pdata_->paddrinfo_res0_))
    {
        pdata_->addrinfo_ip_ = ip;
        pdata_->addrinfo_port_ = port;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////
// class name : DgramSocket
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
DgramSocket::DgramSocket(Family family, int protocol, size_t recv_buf_len) :
    socket_(family, SockType_Dgram, protocol),
    recv_buf_(0),
    recv_buf_len_(recv_buf_len)
{
    recv_buf_ = new char[recv_buf_len_];
    memset(recv_buf_, 0, recv_buf_len_);
}

DgramSocket::~DgramSocket()
{
    safeDeleteArray(recv_buf_);
}

bool DgramSocket::init()
{
    return socket_.init();
}

int DgramSocket::setBlock(unsigned long block)
{
    return socket_.setBlock(block);
}

int DgramSocket::setSockopt(int level, int optname, const void* optval, int optlen)
{
    return socket_.setSockopt(level, optname, optval, optlen);
}

//////////////////////////////////////////////////
// class name : StreamSocket
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
typedef std::vector<TConnSocket*> ConnVec;

struct StreamSocket::StreamSocketImpl
{
    StreamSocketImpl(Family family,
                     StreamSocketSvrListener* sslistener,
                     int protocol,
                     size_t recv_buf_len) :
        socket_(family, SockType_Stream, protocol),
        sslistener_(sslistener),
        recv_buf_(0),
        recv_buf_len_(recv_buf_len),
        is_selecting_(false)
    {
        recv_buf_ = new char[recv_buf_len_];
        memset(recv_buf_, 0, sizeof(recv_buf_));
    }

    ~StreamSocketImpl()
    {
        clearConnVec();
        safeDeleteArray(recv_buf_);
    }

    int getSocket() const
    {
        return socket_.getSocket();
    }

    char* getRecvBuf() const
    {
        return recv_buf_;
    }

    int setBlock(unsigned long block)
    {
        return socket_.setBlock(block);
    }

    int setSockopt(int level, int optname, const void* optval, int optlen)
    {
        return socket_.setSockopt(level, optname, optval, optlen);
    }

    int newConnectionHandler(fd_set* poreadfds)
    {
        TConnSocket* pcs = new TConnSocket();
        int connection = socket_.accept(&pcs->taddrinfo);

        if (kInvalidSocket != connection)
        {
            int conn_no = setConnection(pcs);
            if(conn_no >= 0)
            {
                pcs->socket = connection;

                FD_SET((unsigned int)connection, poreadfds);

                if (sslistener_)
                    sslistener_->newConn(conn_no, conn_socket_vec_.at(conn_no));

                return connection;
            }
            else
            {
                util::closeSocket(connection);
                trace(Trace::Error, "<util::net> setConnection failed, listen queue is full.");
            }
        }

        safeDelete(pcs);
        return kInvalidSocket;
    }

    void existConnectionHandler(fd_set* preadfds, fd_set* poredfds)
    {
        for (size_t i = 0; i < conn_socket_vec_.size(); ++i)
        {
            if (0 == conn_socket_vec_.at(i))
                continue;
            if (!FD_ISSET(conn_socket_vec_.at(i)->socket, preadfds))
                continue;

            memset(recv_buf_, 0, recv_buf_len_);
            int len = ::recv(conn_socket_vec_.at(i)->socket, recv_buf_, recv_buf_len_, 0);
            if (len <= 0)
            {
                FD_CLR((unsigned int)conn_socket_vec_.at(i)->socket, poredfds);
                closeConn(i);
                continue;
            }

              if (sslistener_)
                  sslistener_->recvConn(recv_buf_, len, i, conn_socket_vec_.at(i));
        }
    }

    void clearConnVec()
    {
        ConnVec::iterator it;
        for (it = conn_socket_vec_.begin(); it != conn_socket_vec_.end(); ++it)
            safeDelete(*it);
        conn_socket_vec_.clear();
    }

    int findEmptyPos()
    {
        for (size_t i = 0; i < conn_socket_vec_.size(); ++i)
        {
            if (0 == conn_socket_vec_[i])
                return i;
        }
        return -1;
    }

    bool setConnToEmptyPos(int pos, TConnSocket* ptconn_sock)
    {
        if (pos < 0 || pos >= (int)conn_socket_vec_.size())
            return false;

        if (0 != conn_socket_vec_[pos])
            return false;

        conn_socket_vec_[pos] = ptconn_sock;
        return true;
    }

    int setConnection(TConnSocket* ptconn_sock)
    {
        for (size_t i = 0; i < conn_socket_vec_.size(); ++i)
        {
            if (0 == conn_socket_vec_.at(i))
            {
                conn_socket_vec_[i] = ptconn_sock;
                return i;
            }
        }
        return -1;
    }

    bool closeConn(size_t conn)
    {
        if (conn >= conn_socket_vec_.size())
            return false;
        if (0 == conn_socket_vec_.at(conn))
            return false;

        int ret = ::closesocket(conn_socket_vec_.at(conn)->socket);
        safeDelete(conn_socket_vec_.at(conn));
        conn_socket_vec_[conn] = 0;

        if (sslistener_)
            sslistener_->closeConn(conn);

        return (0 == ret) ? true : false;
    }

    bool shutdownConn(size_t conn)
    {
        return 0 == ::shutdown(conn_socket_vec_.at(conn)->socket, 2);
    }

    TConnSocket getTConnSocket(size_t conn)
    {
        if (conn >= conn_socket_vec_.size())
            return TConnSocket();
        if (0 == conn_socket_vec_.at(conn))
            return TConnSocket();

        return TConnSocket(*conn_socket_vec_.at(conn));
    }

    int getConnSocket(size_t conn)
    {
        if (conn >= conn_socket_vec_.size())
            return kInvalidSocket;
        if (0 == conn_socket_vec_.at(conn))
            return kInvalidSocket;

        return conn_socket_vec_.at(conn)->socket;
    }

    size_t getConnSocketCollSize() const
    { return conn_socket_vec_.size(); }

    size_t getConnCnt()
    {
        ConnVec::iterator it;
        size_t cnt = 0;
        for (it = conn_socket_vec_.begin(); it != conn_socket_vec_.end(); ++it)
        {
            if (0 != *it)
                ++cnt;
        }
        return cnt;
    }

    Socket socket_;
    StreamSocketSvrListener* sslistener_;
    char* recv_buf_;
    size_t recv_buf_len_;
    ConnVec conn_socket_vec_;
    bool is_selecting_;
};

StreamSocket::StreamSocket(Family family,
                             StreamSocketSvrListener* sslistener,
                             int protocol,
                             size_t recv_buf_len) :
    pimpl_(new StreamSocketImpl(family,
                                sslistener,
                                protocol,
                                recv_buf_len))
{}

StreamSocket::~StreamSocket()
{}

int StreamSocket::getSocket() const
{ return pimpl_->getSocket(); }

char* StreamSocket::getRecvBuf() const
{ return pimpl_->getRecvBuf(); }

size_t StreamSocket::getConnSocketCollSize() const
{ return pimpl_->getConnSocketCollSize(); }

size_t StreamSocket::getConnCnt()
{ return pimpl_->getConnCnt(); }

TConnSocket StreamSocket::getTConnSocket(size_t conn)
{ return pimpl_->getTConnSocket(conn); }

int StreamSocket::getConnSocket(size_t conn)
{ return pimpl_->getConnSocket(conn); }

int StreamSocket::setBlock(unsigned long block)
{ return pimpl_->setBlock(block); }

int StreamSocket::setSockopt(int level, int optname, const void* optval, int optlen)
{ return pimpl_->setSockopt(level, optname, optval, optlen); }


void StreamSocket::prepareRecvMsg(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket, const MsgInfo& msg_info)
{
    size_t total_len = len;
    //head recv
    total_len = recvEnoughBytes(buf, total_len, msg_info.header_len_, conn);
    if (0 == total_len)
        return;

    size_t msg_len = getMsgLenFromHeader(buf, msg_info);
    if (0 == msg_len)
        return;

    //body recv
    total_len = recvEnoughBytes(buf, total_len, msg_len, conn);
    if (0 == total_len)
        return;

    //process
    if (pimpl_->sslistener_)
        pimpl_->sslistener_->processMsgConn((char*)buf, msg_len, conn, ptconn_socket);


    if (msg_len < total_len)
    {
       memmove(buf, buf + msg_len, total_len - msg_len);
       prepareRecvMsg(buf, total_len - msg_len, conn, ptconn_socket, msg_info);
    }
}

void StreamSocket::prepareRecvMsg(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket, size_t msg_len)
{
    //recv msg
    size_t total_len = recvEnoughBytes(buf, len, msg_len, conn);
    if (0 == total_len)
        return;

    //process
    if (pimpl_->sslistener_)
        pimpl_->sslistener_->processMsgConn((char*)buf, msg_len, conn, ptconn_socket);

    if (msg_len < total_len)
    {
       memmove(buf, buf + msg_len, total_len - msg_len);
       prepareRecvMsg(buf, total_len - msg_len, conn, ptconn_socket, msg_len);
    }
}

size_t StreamSocket::getMsgLenFromHeader(char* buf, const MsgInfo& msg_info)
{
    switch (msg_info.body_len_field_type_size_)
    {
    case 1:
        return msg_info.header_len_ + *(uint8_t*)(buf + msg_info.body_len_field_offset_);
    case 2:
        {
            if (msg_info.net_byte_order_)
                return msg_info.header_len_ + u_ntohs(*(uint16_t*)(buf + msg_info.body_len_field_offset_));
            else
                return msg_info.header_len_ + *(uint16_t*)(buf + msg_info.body_len_field_offset_);
        }
    case 4:
        {
            if (msg_info.net_byte_order_)
                return msg_info.header_len_ + u_ntohl(*(uint32_t*)(buf + msg_info.body_len_field_offset_));
            else
                return msg_info.header_len_ + *(uint32_t*)(buf + msg_info.body_len_field_offset_);
        }
    default:
        trace(Trace::Error, "<util::net> body_len_field_type_size = %d.", msg_info.body_len_field_type_size_);
        return 0;
    }

}

size_t StreamSocket::recvEnoughBytes(char* buf, size_t cur_len, size_t need_len, size_t conn)
{
    size_t total_len = cur_len;
    while (total_len < need_len)
    {
        size_t len_next = recvConn(conn, buf + total_len, pimpl_->recv_buf_len_ - total_len);
        if (len_next <= 0)
        {
            pimpl_->closeConn(conn);
            return 0;
        }
        total_len += len_next;
    }
    return total_len;
}

//common
bool StreamSocket::isOk() const
{ return pimpl_->socket_.isOk(); }

bool StreamSocket::init()
{ return pimpl_->socket_.init(); }

bool StreamSocket::bind(const std::string& ip, unsigned short port, unsigned short* pport_out)
{ return pimpl_->socket_.bind(ip, port, pport_out); }

bool StreamSocket::closeSocket()
{
    pimpl_->is_selecting_ = false;
    return pimpl_->socket_.closeSocket();
}

//server
int StreamSocket::accept(struct TIpAddrInfo* pout_tipaddrinfo)
{ return pimpl_->socket_.accept(pout_tipaddrinfo); }

int StreamSocket::sendConn(size_t conn, const char* buf, size_t len, int flag)
{ return pimpl_->socket_.send(getTConnSocket(conn).socket, buf, len, flag); }

int StreamSocket::recvConn(size_t conn, char* buf, size_t len, int flag)
{ return pimpl_->socket_.recv(getTConnSocket(conn).socket, buf, len, flag); }

int StreamSocket::serverSend(int client_sock, const char* buf, size_t len, int flag)
{ return pimpl_->socket_.send(client_sock, buf, len, flag); }

int StreamSocket::serverRecv(int client_sock, char* buf, size_t len, int flag)
{ return pimpl_->socket_.recv(client_sock, buf, len, flag); }

int StreamSocket::serverRecv(int client_sock, int flag)
{ return pimpl_->socket_.recv(client_sock, pimpl_->recv_buf_, pimpl_->recv_buf_len_, flag); }

bool StreamSocket::closeConn(size_t conn)
{ return pimpl_->closeConn(conn); }

bool StreamSocket::shutdownConn(size_t conn)
{ return pimpl_->shutdownConn(conn); }

bool StreamSocket::listen(size_t backlog)
{
    pimpl_->clearConnVec();
    bool ret = pimpl_->socket_.listen(backlog);
    if (ret)
        pimpl_->conn_socket_vec_.resize(backlog);
    return ret;
}

void StreamSocket::startServer(size_t ms)
{
    int maxsfd = kInvalidSocket;
    fd_set readfds, oreadfds;

    FD_ZERO(&oreadfds);
    FD_SET((unsigned int)pimpl_->socket_.getSocket(), &oreadfds);
    maxsfd = pimpl_->socket_.getSocket();

    pimpl_->is_selecting_ = true;

    struct timeval tv = getTimeVal(ms);
    struct timeval* ptv = &tv;
    if (0 == ms)
        ptv = 0;
    while (pimpl_->is_selecting_)
    {
        readfds = oreadfds;

        int n = ::select((int)maxsfd + 1, &readfds, 0, 0, ptv);
        if (n < 0)
        {
            continue;
        }
        else if (0 == n)
        {
            continue;
        }

        if (FD_ISSET(pimpl_->socket_.getSocket(), &readfds))
        {
            int connection = pimpl_->newConnectionHandler(&oreadfds);
            if (kInvalidSocket != connection)
            {
                if (maxsfd < connection)
                    maxsfd = connection;
            }
        }
        else
        {
            pimpl_->existConnectionHandler(&readfds, &oreadfds);
        }
    }
}


//client
bool StreamSocket::connect(const std::string& ip, unsigned short port)
{ return pimpl_->socket_.connect(ip, port); }

int StreamSocket::clientSend(const char* buf, size_t len, int flag)
{ return pimpl_->socket_.send(getSocket(), buf, len, flag); }

int StreamSocket::clientRecv(char* buf, int len, int flag)
{ return pimpl_->socket_.recv(getSocket(), buf, len, flag); }

int StreamSocket::clientRecv(int flag)
{
    memset(pimpl_->recv_buf_, 0, pimpl_->recv_buf_len_);
    return pimpl_->socket_.recv(getSocket(), pimpl_->recv_buf_, pimpl_->recv_buf_len_, flag);
}

} // namespace util
