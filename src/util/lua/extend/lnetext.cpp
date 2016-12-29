#include "lnetext.hpp"
#include "util/luaextend.hpp"
#include "util/net.hpp"

namespace util
{

const std::string kUdpHandle = "Udp*";
const std::string kTcpHandle = "Tcp*";
const std::string kRawHandle = "Raw*";

static int lua_htons(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_htons((uint16_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int lua_ntohs(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_ntohs((uint16_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int lua_htonl(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_htonl((uint32_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int lua_ntohl(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_ntohl((uint32_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int ip_array_to_str(lua_State* plua_state)
{
    luaPushString(plua_state, ipArrayToStr((Family)luaGetInteger(plua_state, 1, Family_None),
                                          (unsigned char*)luaGetLightUserData(plua_state, 2, 0),
                                          (size_t)luaGetInteger(plua_state, 3, 0)));
    return 1;
}

static int ip_str_to_array(lua_State* plua_state)
{
    luaPushLightUserData(plua_state, ipStrToArray((Family)luaGetInteger(plua_state, 1, Family_None),
                                                  luaGetString(plua_state, 2, ""),
                                                  (unsigned char*)luaGetLightUserData(plua_state, 3, 0),
                                                  (size_t)luaGetInteger(plua_state, 4, 0)));
    return 1;
}

////////////////////////////////////////////////////
// UDP
////////////////////////////////////////////////////
static int udpCreate(lua_State* plua_state)
{
    LuaObject<DgramSocket>* p = luaNewEmptyObject<DgramSocket>(plua_state, kUdpHandle);
    p->setData(new DgramSocket((Family)luaGetInteger(plua_state, 1, Family_IPv4)));

    return 1;
}

static int udpDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<DgramSocket>(plua_state, kUdpHandle);
}

static int udpGetSocket(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushInteger(plua_state, pds->getSocket());

    return 1;
}

static int udpInit(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushBoolean(plua_state, pds->init());

    return 1;
}

static int udpIsOk(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushBoolean(plua_state, pds->isOk());

    return 1;
}

static int udpSetBlock(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushInteger(plua_state, pds->setBlock(luaGetBoolean(plua_state, 2, true)));

    return 1;
}

static int udpBind(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushBoolean(plua_state, pds->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int udpRecvFrom(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);

    std::string ip("");
    unsigned short port(0);

    luaExtendAssert(plua_state, kLuaExtendLibUtil, "udpRecvFrom", pds, "null pointer");

    luaPushInteger(plua_state, pds->recvFrom(&ip, &port));
    luaPushString(plua_state, ip);
    luaPushInteger(plua_state, port);

    return 3;
}

static int udpSendTo(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);

    if (LuaString == luaGetType(plua_state, 2))
    {
        std::string str = luaGetString(plua_state, 2);
        luaPushInteger(plua_state, pds->sendTo(str.c_str(),
                                              luaGetInteger(plua_state, 3, str.length()),
                                              luaGetString(plua_state, 4, "127.0.0.1"),
                                              luaGetInteger(plua_state, 5, 0)));
    }
    else if (LuaLightUserData == luaGetType(plua_state, 2))
    {
        luaPushInteger(plua_state, pds->sendTo((const char*)luaGetLightUserData(plua_state, 2),
                                              luaGetInteger(plua_state, 3, 0),
                                              luaGetString(plua_state, 4, "127.0.0.1"),
                                              luaGetInteger(plua_state, 5, 0)));
    }
    else
    {
        luaPushInteger(plua_state, -1);
    }

    return 1;
}

static int udpGetRecvBuf(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushLightUserData(plua_state, (void*)pds->getRecvBuf());

    return 1;
}

static int udpClose(lua_State* plua_state)
{
    DgramSocket* pds = luaGetObjectData<DgramSocket>(plua_state, kUdpHandle);
    luaPushBoolean(plua_state, pds->closeSocket());

    return 1;
}

static int udpToString(lua_State* plua_state)
{
    return luaObjectToString<DgramSocket>(plua_state, kUdpHandle);
}

////////////////////////////////////////////////////
// TCP
////////////////////////////////////////////////////

static int tcpCreate(lua_State* plua_state)
{    
    LuaObject<StreamSocket>* p = luaNewEmptyObject<StreamSocket>(plua_state, kTcpHandle);
    p->setData(new StreamSocket((Family)luaGetInteger(plua_state, 1, Family_IPv4)));

    return 1;
}

static int tcpDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<StreamSocket>(plua_state, kTcpHandle);
}

static int tcpGetSocket(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushInteger(plua_state, pss->getSocket());

    return 1;
}

static int tcpInit(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->init());

    return 1;
}

static int tcpIsOk(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->isOk());

    return 1;
}

static int tcpSetBlock(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushInteger(plua_state, pss->setBlock(luaGetBoolean(plua_state, 2, true)));

    return 1;
}

static int tcpBind(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpListen(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->listen(luaGetInteger(plua_state, 2, 5)));

    return 1;
}

static int tcpAccept(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    TIpAddrInfo tipaddrinfo;
    luaPushInteger(plua_state, pss->accept(&tipaddrinfo));

    return 1;
}

static int tcpConnect(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpClientRecv(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushInteger(plua_state, pss->clientRecv());

    return 1;
}

static int tcpClientSend(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    
    if (LuaString == luaGetType(plua_state, 2))
    {
        std::string str = luaGetString(plua_state, 2);
        luaPushInteger(plua_state, pss->clientSend(str.c_str(),
                                              luaGetInteger(plua_state, 3, str.length())
                                              ));
    }
    else if (LuaLightUserData == luaGetType(plua_state, 2))
    {
        luaPushInteger(plua_state, pss->clientSend((const char*)luaGetLightUserData(plua_state, 2),
                                              luaGetInteger(plua_state, 3, 0)
                                              ));
    }
    else
    {
        luaPushInteger(plua_state, -1);
    }

    return 1;
}

static int tcpServerRecv(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    
    int client_sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    if (kInvalidSocket == client_sock)
    {
        luaPushInteger(plua_state, -1);
        return 1;
    }
    luaPushInteger(plua_state, pss->serverRecv(client_sock));

    return 1;
}

static int tcpServerSend(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    
    int client_sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "tcpServerSend", kInvalidSocket != client_sock, "invalid client socket");

    if (LuaString == luaGetType(plua_state, 3))
    {
        std::string str = luaGetString(plua_state, 3);
        luaPushInteger(plua_state, pss->serverSend(client_sock,
                                                  str.c_str(),
                                                  luaGetInteger(plua_state, 4, str.length())
                                                  ));
    }
    else if (LuaLightUserData == luaGetType(plua_state, 3))
    {
        luaPushInteger(plua_state, pss->serverSend(client_sock,
                                                (const char*)luaGetLightUserData(plua_state, 3),
                                                luaGetInteger(plua_state, 4, 0)
                                                ));
    }
    else
    {
        luaPushInteger(plua_state, -1);
    }

    return 1;
}

static int tcpGetRecvBuf(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushLightUserData(plua_state, (void*)pss->getRecvBuf());

    return 1;
}

static int tcpClose(lua_State* plua_state)
{
    StreamSocket* pss = luaGetObjectData<StreamSocket>(plua_state, kTcpHandle);
    luaPushBoolean(plua_state, pss->closeSocket());

    return 1;
}

static int tcpToString(lua_State* plua_state)
{
    return luaObjectToString<StreamSocket>(plua_state, kTcpHandle);
}

////////////////////////////////////////////////////
// RAW
////////////////////////////////////////////////////
static int rawCreate(lua_State* plua_state)
{
    LuaObject<Socket>* p = luaNewEmptyObject<Socket>(plua_state, kRawHandle);
    p->setData(new Socket((Family)luaGetInteger(plua_state, 1, Family_IPv4), SockType_Raw, luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int rawDestroy(lua_State* plua_state)
{
    return luaObjectDestroy<Socket>(plua_state, kRawHandle);
}

static int rawGetSocket(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushInteger(plua_state, ps->getSocket());

    return 1;
}

static int rawInit(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->init());

    return 1;
}

static int rawIsOk(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->isOk());

    return 1;
}

static int rawSetBlock(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushInteger(plua_state, ps->setBlock(luaGetBoolean(plua_state, 2, true)));

    return 1;
}

static int rawBind(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecvFrom(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 2, 0));
    size_t len = static_cast<size_t>(luaGetInteger(plua_state, 3, 0));

    std::string ip("");
    unsigned short port(0);

    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawRecvFrom", ps, "null pointer");

    luaPushInteger(plua_state, ps->recvFrom(buf, len, &ip, &port));
    luaPushString(plua_state, ip);
    luaPushInteger(plua_state, port);

    return 3;
}

static int rawSendTo(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    
    if (LuaString == luaGetType(plua_state, 2))
    {
        std::string str = luaGetString(plua_state, 2);
        luaPushInteger(plua_state, ps->sendTo(str.c_str(),
                                              luaGetInteger(plua_state, 3, str.length()),
                                              luaGetString(plua_state, 4, "127.0.0.1"),
                                              luaGetInteger(plua_state, 5, 0)));
    }
    else if (LuaLightUserData == luaGetType(plua_state, 2))
    {
        luaPushInteger(plua_state, ps->sendTo((const char*)luaGetLightUserData(plua_state, 2),
                                              luaGetInteger(plua_state, 3, 0),
                                              luaGetString(plua_state, 4, "127.0.0.1"),
                                              luaGetInteger(plua_state, 5, 0)));
    }
    else
    {
        luaExtendError(plua_state, kLuaExtendLibUtil, "rawSendTo", "parameter 2 is not string or buffer");
    }

    return 1;
}

static int rawListen(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->listen(luaGetInteger(plua_state, 2, 5)));

    return 1;
}

static int rawAccept(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    
    TIpAddrInfo tipaddrinfo;
    luaPushBoolean(plua_state, true);
    std::string ip = ipArrayToStr((Family)tipaddrinfo.taddr.ipaddr_ver,
                     (unsigned char*)&tipaddrinfo.taddr.ipaddr,
                     16);

    luaPushString(plua_state, ip);
    luaPushInteger(plua_state, tipaddrinfo.port);

    return 3;
}

static int rawConnect(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecv(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 3, 0));
    size_t len = static_cast<size_t>(luaGetInteger(plua_state, 4, 0));

    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawRecv", ps, "null pointer");
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawRecv", kInvalidSocket != sock, "invalid socket");
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawRecv", NULL != buf, "buffer is NULL");

    luaPushInteger(plua_state, ps->recv(sock, buf, len));

    return 1;
}

static int rawSend(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawSend", ps, "null pointer");
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "rawSend", kInvalidSocket != sock, "invalid socket");

    if (LuaString == luaGetType(plua_state, 3))
    {
        std::string str = luaGetString(plua_state, 3);
        luaPushInteger(plua_state, ps->send(sock,
                                            str.c_str(),
                                            luaGetInteger(plua_state, 4, str.length())
                                            ));
    }
    else if (LuaLightUserData == luaGetType(plua_state, 3))
    {
        luaPushInteger(plua_state, ps->send(sock,
                                            (const char*)luaGetLightUserData(plua_state, 3),
                                            luaGetInteger(plua_state, 4, 0)
                                            ));
    }
    else
    {
        luaPushInteger(plua_state, -1);
    }

    return 1;
}

static int rawClose(lua_State* plua_state)
{
    Socket* ps = luaGetObjectData<Socket>(plua_state, kRawHandle);
    luaPushBoolean(plua_state, ps->closeSocket());

    return 1;
}

static int rawToString(lua_State* plua_state)
{
    return luaObjectToString<Socket>(plua_state, kRawHandle);
}

static int close_socket(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "closeSocket",
        luaGetTop(plua_state) == 1 && LuaNumber == luaGetType(plua_state, 1), "invalid socket");

    luaPushInteger(plua_state, closeSocket(luaGetInteger(plua_state, 1)));

    return 1;
}

static const LuaReg net_lib[] =
{    
    {"newUdpSocket", udpCreate},
    {"newTcpSocket", tcpCreate},
    {"newRawSocket", rawCreate},
    
    {"ipArrayToStr", ip_array_to_str},
    {"ipStrToArray", ip_str_to_array},
    {"htons", lua_htons},
    {"ntohs", lua_ntohs},
    {"htonl", lua_htonl},
    {"ntohl", lua_ntohl},
    {"closeSocket", close_socket},

    {0, 0}
};

static const LuaReg udp_obj_lib[] = {
    {"delete", udpDestroy},
    {"getSocket", udpGetSocket},
    {"init", udpInit},
    {"isOk", udpIsOk},
    {"setBlock", udpSetBlock},
    {"bind", udpBind},
    {"recvFrom", udpRecvFrom},
    {"sendTo", udpSendTo},
    {"getRecvBuf", udpGetRecvBuf},
    {"close", udpClose},
    {"__gc", udpDestroy},
    {"__tostring", udpToString},
    
    {0, 0}
};

static const LuaReg tcp_obj_lib[] = {
    {"delete", tcpDestroy},
    {"getSocket", tcpGetSocket},
    {"init", tcpInit},
    {"isOk", tcpIsOk},
    {"setBlock", tcpSetBlock},
    {"bind", tcpBind},
    {"listen", tcpListen},
    {"accept", tcpAccept},
    {"connect", tcpConnect},
    {"clientRecv", tcpClientRecv},
    {"clientSend", tcpClientSend},
    {"serverRecv", tcpServerRecv},
    {"serverSend", tcpServerSend},
    {"getRecvBuf", tcpGetRecvBuf},
    {"close", tcpClose},
    {"__gc", tcpDestroy},
    {"__tostring", tcpToString},
    
    {0, 0}
};

static const LuaReg raw_obj_lib[] = {
    {"delete", rawDestroy},
    {"getSocket", rawGetSocket},
    {"init", rawInit},
    {"isOk", rawIsOk},
    {"setBlock", rawSetBlock},
    {"bind", rawBind},
    {"recvFrom", rawRecvFrom},
    {"sendTo", rawSendTo},
    {"close", rawClose},
    {"listen", rawListen},
    {"accept", rawAccept},
    {"connect", rawConnect},
    {"recv", rawRecv},
    {"send", rawSend},
    {"__gc", rawDestroy},
    {"__tostring", rawToString},
    
    {0, 0}
};

void extendNet(lua_State* plua_state)
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)net_lib);
    luaCreateMeta(plua_state, kUdpHandle, (LuaReg*)udp_obj_lib);    
    luaCreateMeta(plua_state, kTcpHandle, (LuaReg*)tcp_obj_lib);
    luaCreateMeta(plua_state, kRawHandle, (LuaReg*)raw_obj_lib);
}

}
