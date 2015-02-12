#include "lnetlib.hpp"
#include "util/luaextend.hpp"
#include "util/net.hpp"

namespace util
{

////////////////////////////////////////////////////
// NET
////////////////////////////////////////////////////
static int htons(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_htons((uint16_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int ntohs(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_ntohs((uint16_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int htonl(lua_State* plua_state)
{
    luaPushInteger(plua_state, u_htonl((uint32_t)luaGetInteger(plua_state, 1, 0)));
    return 1;
}

static int ntohl(lua_State* plua_state)
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
    size_t family = luaGetInteger(plua_state, 1, Family_IPv4);

    DgramSocket* pds = new DgramSocket((Family)family);

    //LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pds, deleteVoid<DgramSocket>);
    LuaHeapRecyclerManager::getInstance().addHeapObject<DgramSocket>(plua_state, (void*)pds);

    luaPushLightUserData(plua_state, (void*)pds);

    return 1;
}

static int udpDestroy(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pds);

    if (pds)
        delete pds;

    return 0;
}

static int udpGetSocket(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpGetSocket", pds, "null pointer");

    luaPushInteger(plua_state, pds->getSocket());

    return 1;
}

static int udpInit(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpInit", pds, "null pointer");

    luaPushBoolean(plua_state, pds->init());

    return 1;
}

static int udpIsOk(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpIsOk", pds, "null pointer");

    luaPushBoolean(plua_state, pds->isOk());

    return 1;
}

static int udpSetBlock(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpSetBlock", pds, "null pointer");

    luaPushInteger(plua_state, pds->setBlock(luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int udpBind(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpBind", pds, "null pointer");

    luaPushBoolean(plua_state,
        pds->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int udpRecvFrom(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));

    std::string ip("");
    unsigned short port(0);

    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpRecvFrom", pds, "null pointer");

    luaPushInteger(plua_state, pds->recvFrom(&ip, &port));
    luaPushString(plua_state, ip);
    luaPushInteger(plua_state, port);

    return 3;
}

static int udpSendTo(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpSendTo", pds, "null pointer");

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
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpGetRecvBuf", pds, "null pointer");

    luaPushLightUserData(plua_state, (void*)pds->getRecvBuf());

    return 1;
}

static int udpClose(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "udpClose", pds, "null pointer");

    luaPushBoolean(plua_state, pds->closeSocket());

    return 1;
}

////////////////////////////////////////////////////
// TCP
////////////////////////////////////////////////////

static int tcpCreate(lua_State* plua_state)
{
    size_t family = luaGetInteger(plua_state, 1, Family_IPv4);

    StreamSocket* pss = new StreamSocket((Family)family);

    //LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pss, deleteVoid<StreamSocket>);
    LuaHeapRecyclerManager::getInstance().addHeapObject<StreamSocket>(plua_state, (void*)pss);

    luaPushLightUserData(plua_state, (void*)pss);

    return 1;
}

static int tcpDestroy(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pss);

    if (pss)
        delete pss;

    return 0;
}

static int tcpGetSocket(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpGetSocket", pss, "null pointer");

    luaPushInteger(plua_state, pss->getSocket());

    return 1;
}

static int tcpInit(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpInit", pss, "null pointer");

    luaPushBoolean(plua_state, pss->init());

    return 1;
}

static int tcpIsOk(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpIsOk", pss, "null pointer");

    luaPushBoolean(plua_state, pss->isOk());

    return 1;
}

static int tcpSetBlock(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpSetBlock", pss, "null pointer");

    luaPushInteger(plua_state, pss->setBlock(luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int tcpBind(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpBind", pss, "null pointer");

    luaPushBoolean(plua_state,
        pss->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpListen(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpListen", pss, "null pointer");

    luaPushBoolean(plua_state, pss->listen(luaGetInteger(plua_state, 2, 5)));

    return 1;
}

static int tcpAccept(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpAccept", pss, "null pointer");

    TIpAddrInfo tipaddrinfo;
    luaPushInteger(plua_state, pss->accept(&tipaddrinfo));

    return 1;
}

static int tcpConnect(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpConnect", pss, "null pointer");

    luaPushBoolean(plua_state,
        pss->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpClientRecv(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpClientRecv", pss, "null pointer");

    luaPushInteger(plua_state, pss->clientRecv());

    return 1;
}

static int tcpClientSend(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpClientSend", pss, "null pointer");

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
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpServerRecv", pss, "null pointer");

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
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpServerSend", pss, "null pointer");

    int client_sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    if (kInvalidSocket == client_sock)
    {
        luaPushInteger(plua_state, -1);
        return 1;
    }

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
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpGetRecvBuf", pss, "null pointer");

    luaPushLightUserData(plua_state, (void*)pss->getRecvBuf());

    return 1;
}

static int tcpClose(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "tcpClose", pss, "null pointer");

    luaPushBoolean(plua_state, pss->closeSocket());

    return 1;
}

////////////////////////////////////////////////////
// RAW
////////////////////////////////////////////////////
static int rawCreate(lua_State* plua_state)
{
    Socket* ps = new Socket((Family)luaGetInteger(plua_state, 1, Family_IPv4), SockType_Raw, luaGetInteger(plua_state, 2, 0));

    //LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)ps, deleteVoid<Socket>);
    LuaHeapRecyclerManager::getInstance().addHeapObject<Socket>(plua_state, (void*)ps);

    luaPushLightUserData(plua_state, (void*)ps);

    return 1;
}

static int rawDestroy(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)ps);

    if (ps)
        delete ps;

    return 0;
}

static int rawGetSocket(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawGetSocket", ps, "null pointer");

    luaPushInteger(plua_state, ps->getSocket());

    return 1;
}

static int rawInit(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawInit", ps, "null pointer");

    luaPushBoolean(plua_state, ps->init());

    return 1;
}

static int rawIsOk(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawIsOk", ps, "null pointer");

    luaPushBoolean(plua_state, ps->isOk());

    return 1;
}

static int rawSetBlock(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawSetBlock", ps, "null pointer");

    luaPushInteger(plua_state, ps->setBlock(luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int rawBind(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawBind", ps, "null pointer");

    luaPushBoolean(plua_state,
        ps->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecvFrom(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 2, 0));
    size_t len = static_cast<size_t>(luaGetInteger(plua_state, 3, 0));

    std::string ip("");
    unsigned short port(0);

    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawRecvFrom", ps, "null pointer");

    luaPushInteger(plua_state, ps->recvFrom(buf, len, &ip, &port));
    luaPushString(plua_state, ip);
    luaPushInteger(plua_state, port);

    return 3;
}

static int rawSendTo(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawSendTo", ps, "null pointer");

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
        luaExtendError(plua_state, kLuaExtendLibNet, "rawSendTo", "parameter 2 is not string or buffer");
    }

    return 1;
}

static int rawListen(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawListen", ps, "null pointer");

    luaPushBoolean(plua_state, ps->listen(luaGetInteger(plua_state, 2, 5)));

    return 1;
}

static int rawAccept(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawAccept", ps, "null pointer");

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
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawConnect", ps, "null pointer");
    luaPushBoolean(plua_state,
          ps->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecv(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 3, 0));
    size_t len = static_cast<size_t>(luaGetInteger(plua_state, 4, 0));

    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawRecv", ps, "null pointer");
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawRecv", kInvalidSocket != sock, "invalid socket");
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawRecv", NULL != buf, "buffer is NULL");

    luaPushInteger(plua_state, ps->recv(sock, buf, len));

    return 1;
}

static int rawSend(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawSend", ps, "null pointer");
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawSend", kInvalidSocket != sock, "invalid socket");

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
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibNet, "rawClose", ps, "null pointer");

    luaPushBoolean(plua_state, ps->closeSocket());

    return 1;
}

//close_socket

static int close_socket(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibNet, "closeSocket",
        luaGetTop(plua_state) == 1 && LuaNumber == luaGetType(plua_state, 1), "invalid socket");

    luaPushInteger(plua_state, closeSocket(luaGetInteger(plua_state, 1)));

    return 1;
}

static const u_luaL_Reg net_lib[] =
{
    {"ipArrayToStr", ip_array_to_str},
    {"ipStrToArray", ip_str_to_array},
    {"htons", htons},
    {"ntohs", ntohs},
    {"htonl", htonl},
    {"ntohl", ntohl},

    {"udpCreate", udpCreate},
    {"udpDestroy", udpDestroy},
    {"udpGetSocket", udpGetSocket},
    {"udpInit", udpInit},
    {"udpIsOk", udpIsOk},
    {"udpSetBlock", udpSetBlock},
    {"udpBind", udpBind},
    {"udpRecvFrom", udpRecvFrom},
    {"udpSendTo", udpSendTo},
    {"udpGetRecvBuf", udpGetRecvBuf},
    {"udpClose", udpClose},

    {"tcpCreate", tcpCreate},
    {"tcpDestroy", tcpDestroy},
    {"tcpGetSocket", tcpGetSocket},
    {"tcpInit", tcpInit},
    {"tcpIsOk", tcpIsOk},
    {"tcpSetBlock", tcpSetBlock},
    {"tcpBind", tcpBind},
    {"tcpListen", tcpListen},
    {"tcpAccept", tcpAccept},
    {"tcpConnect", tcpConnect},
    {"tcpClientRecv", tcpClientRecv},
    {"tcpClientSend", tcpClientSend},
    {"tcpServerRecv", tcpServerRecv},
    {"tcpServerSend", tcpServerSend},
    {"tcpGetRecvBuf", tcpGetRecvBuf},
    {"tcpClose", tcpClose},

    {"rawCreate", rawCreate},
    {"rawDestroy", rawDestroy},
    {"rawGetSocket", rawGetSocket},
    {"rawInit", rawInit},
    {"rawIsOk", rawIsOk},
    {"rawSetBlock", rawSetBlock},
    {"rawBind", rawBind},
    {"rawRecvFrom", rawRecvFrom},
    {"rawSendTo", rawSendTo},
    {"rawClose", rawClose},
    {"rawListen", rawListen},
    {"rawAccept", rawAccept},
    {"rawConnect", rawConnect},
    {"rawRecv", rawRecv},
    {"rawSend", rawSend},

    {"closeSocket", close_socket},

    {0, 0}
};

int lualibNetCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)net_lib);
    return 1;
}

}
