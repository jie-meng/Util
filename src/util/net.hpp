#ifndef UTIL_NET_HPP
#define UTIL_NET_HPP

#include "base.hpp"

namespace util
{

static const int kInvalidSocket = -1;

//enums
enum Family
{
    Family_None,
    Family_IPv4,
    Family_IPv6
};

enum SockType
{
    SockType_Dgram,
    SockType_Stream,
    SockType_Raw
};

uint16_t u_htons(uint16_t value);
uint16_t u_ntohs(uint16_t value);
uint32_t u_htonl(uint32_t value);
uint32_t u_ntohl(uint32_t value);
uint64_t u_htonll(uint64_t value);
uint64_t u_ntohll(uint64_t value);

std::string ipArrayToStr(Family family, unsigned char* ip_byte_array, size_t len);
unsigned char* ipStrToArray(Family family, const std::string& ip_str, unsigned char* ip_byte_array, size_t len);

//////////////////////////////////////////////////
// class name : SocketStartup
// description :
// author :
// time : 2013-01-08-21.04
//////////////////////////////////////////////////
class SocketStartup
{
public:
    SINGLETON(SocketStartup)
    bool isOk() const;
private:
    SocketStartup();
    ~SocketStartup();
    struct SocketStartupImpl;
    UtilAutoPtr<SocketStartupImpl> pimpl_;
private:
    DISALLOW_COPY_AND_ASSIGN(SocketStartup)
};

int getLastError();

//structs
struct TIpAddr
{
    TIpAddr()
    {
        ipaddr_ver = Family_None;
        memset(ipaddr, 0, sizeof(ipaddr));
    }
    int ipaddr_ver;
    char ipaddr[16];
};

struct TIpAddrInfo
{
    TIpAddrInfo() :
        port(0)
    {}
    TIpAddr taddr;
    unsigned short port;
};

bool isIpAddrEmpty(const TIpAddr& ip);
bool isIpAddrEqual(const TIpAddr& ip_first, const TIpAddr& ip_second);

int createSocket(Family family, SockType sock_type, int protocol);
int closeSocket(int socket);

//////////////////////////////////////////////////
// class name : Socket
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
class Socket
{
public:
    Socket(Family family, SockType sock_type, int protocol = 0);
    virtual ~Socket();
    int getSocket() const;
    //common
    bool init();
    bool isOk() const;
    bool bind(const std::string& ip, unsigned short port);
    bool closeSocket();
    int setBlock(unsigned long block = 0);
    int setSockopt(int level, int optname, const void* optval, int optlen);
    //dgram
    int sendTo(const char* buf, size_t len, const std::string& ip, unsigned short port, int flag = 0);
    int recvFrom(char* buf, size_t len, std::string* pout_peer_ip, unsigned short* pout_peer_port, int flag = 0);
    //stream
    bool listen(size_t backlog);
    int accept(struct TIpAddrInfo* pout_tipaddrinfo);
    bool connect(const std::string& ip, unsigned short port);
    int send(int socket, const char* buf, size_t len, int flag = 0);
    int recv(int socket, char* buf, size_t len, int flag = 0);
private:
    bool getAddrInfo(const std::string& ip, const std::string& port);
    inline bool getAddrInfo(const std::string& ip, unsigned short port)
    { return getAddrInfo(ip, toString(port)); }
private:
    struct SocketData;
    UtilAutoPtr<SocketData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(Socket)
};

//////////////////////////////////////////////////
// class name : DgramSocket
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
class DgramSocket
{
public:
    explicit DgramSocket(Family family, int protocol = 0, size_t recv_buf_len = kBufSize);
    virtual ~DgramSocket();
    inline int getSocket() const { return socket_.getSocket(); }
    inline char* getRecvBuf() const { return recv_buf_; }
    inline bool isOk() const { return socket_.isOk(); }
    bool init();
    inline bool bind(const std::string& ip, unsigned short port) { return socket_.bind(ip, port); }
    inline int sendTo(const char* buf, size_t len, const std::string& ip, unsigned short port, int flag = 0)
    { return socket_.sendTo(buf, len, ip, port, flag); }
    inline int recvFrom(std::string* pout_peer_ip, unsigned short* pout_peer_port, int flag = 0)
    {  memset(recv_buf_, 0, recv_buf_len_); return socket_.recvFrom(recv_buf_, recv_buf_len_, pout_peer_ip, pout_peer_port, flag); }
    inline bool closeSocket() { return socket_.closeSocket(); }
    int setBlock(unsigned long block = 0);
    int setSockopt(int level, int optname, const void* optval, int optlen);
private:
    Socket socket_;
    char* recv_buf_;
    size_t recv_buf_len_;
private:
    DISALLOW_COPY_AND_ASSIGN(DgramSocket)
};

//////////////////////////////////////////////////
// class name : StreamSocket
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
struct TConnSocket
{
    TConnSocket() :
        socket(kInvalidSocket)
    {}
    int socket;
    TIpAddrInfo taddrinfo;
};

struct MsgInfo
{
    MsgInfo(size_t header_len, size_t body_len_field_offset, size_t body_len_field_type_size, bool net_byte_order) :
        header_len_(header_len),
        body_len_field_offset_(body_len_field_offset),
        body_len_field_type_size_(body_len_field_type_size),
        net_byte_order_(net_byte_order)
        {}
    size_t header_len_;
    size_t body_len_field_offset_;
    size_t body_len_field_type_size_;
    bool net_byte_order_;
};

class StreamSocketSvrListener
{
public:
    StreamSocketSvrListener() {}
    virtual ~StreamSocketSvrListener() {}
    virtual void newConn(size_t conn, TConnSocket* ptconn_socket) {}
    virtual void recvConn(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket) {}
    virtual void closeConn(size_t conn) {}
    virtual void processMsgConn(char* pmsg, size_t len, size_t conn, TConnSocket* ptconn_socket) {}
};

class StreamSocket
{
public:
    explicit StreamSocket(Family family,
                         StreamSocketSvrListener* sslistener = 0,
                         int protocol = 0,
                         size_t recv_buf_len = kBufSize);
    virtual ~StreamSocket();
    int getSocket() const;
    char* getRecvBuf() const;
    size_t getConnSocketCollSize() const;
    size_t getConnCnt();
    TConnSocket getTConnSocket(size_t conn);
    int getConnSocket(size_t conn);

    //msg process
    //void setProcessMsgFunc(ProcessMsgFunc process_msg_func);
    void prepareRecvMsg(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket, const MsgInfo& msg_info);
    void prepareRecvMsg(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket, size_t msg_len);
    //common
    bool isOk() const;
    bool init();
    bool bind(const std::string& ip, unsigned short port);
    bool closeSocket();
    int setBlock(unsigned long block = 0);
    int setSockopt(int level, int optname, const void* optval, int optlen);
    //server
    bool listen(size_t backlog);
    int accept(struct TIpAddrInfo* pout_tipaddrinfo);
    int sendConn(size_t conn, const char* buf, size_t len, int flag = 0);
    int recvConn(size_t conn, char* buf, size_t len, int flag = 0);
    int serverSend(int client_sock, const char* buf, size_t len, int flag = 0);
    int serverRecv(int client_sock, char* buf, size_t len, int flag = 0);
    int serverRecv(int client_sock, int flag = 0);
    void startServer(size_t ms = 0);
    bool closeConn(size_t conn);
    bool shutdownConn(size_t conn);
    //client
    bool connect(const std::string& ip, unsigned short port);
    int clientSend(const char* buf, size_t len, int flag = 0);
    int clientRecv(char* buf, int len, int flag = 0);
    int clientRecv(int flag = 0);
private:
    size_t getMsgLenFromHeader(char* buf, const MsgInfo& msg_info);
    size_t recvEnoughBytes(char* buf, size_t cur_len, size_t need_len, size_t conn);
private:
    struct StreamSocketImpl;
    UtilAutoPtr<StreamSocketImpl> pimpl_;
private:
    DISALLOW_COPY_AND_ASSIGN(StreamSocket)
};

} // namespace util

#endif // UTIL_NET_HPP
