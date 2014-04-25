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
    luaPushUInteger(plua_state, u_htons((uint16_t)luaGetUInteger(plua_state, 1, 0)));
    return 1;
}

static int ntohs(lua_State* plua_state)
{
    luaPushUInteger(plua_state, u_ntohs((uint16_t)luaGetUInteger(plua_state, 1, 0)));
    return 1;
}

static int htonl(lua_State* plua_state)
{
    luaPushUInteger(plua_state, u_htonl((uint32_t)luaGetUInteger(plua_state, 1, 0)));
    return 1;
}

static int ntohl(lua_State* plua_state)
{
    luaPushUInteger(plua_state, u_ntohl((uint32_t)luaGetUInteger(plua_state, 1, 0)));
    return 1;
}

static int ip_array_to_str(lua_State* plua_state)
{
    luaPushString(plua_state, ipArrayToStr((Family)luaGetUInteger(plua_state, 1, Family_None),
                                          (unsigned char*)luaGetLightUserData(plua_state, 2, 0),
                                          (size_t)luaGetUInteger(plua_state, 3, 0)));
    return 1;
}

static int ip_str_to_array(lua_State* plua_state)
{
    luaPushLightUserData(plua_state, ipStrToArray((Family)luaGetUInteger(plua_state, 1, Family_None),
                                                  luaGetString(plua_state, 2, ""),
                                                  (unsigned char*)luaGetLightUserData(plua_state, 3, 0),
                                                  (size_t)luaGetUInteger(plua_state, 4, 0)));
    return 1;
}

////////////////////////////////////////////////////
// UDP
////////////////////////////////////////////////////
static int udpCreate(lua_State* plua_state)
{
    size_t family = luaGetUInteger(plua_state, 1, Family_IPv4);

    DgramSocket* pds = new DgramSocket((Family)family);

    LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pds, deleteVoid<DgramSocket>);

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
    if (!pds)
        luaPushInteger(plua_state, kInvalidSocket);
    else
        luaPushInteger(plua_state, pds->getSocket());

    return 1;
}

static int udpInit(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pds->init());

    return 1;
}

static int udpIsOk(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pds->isOk());

    return 1;
}

static int udpSetBlock(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pds->setBlock(luaGetUInteger(plua_state, 2, 0)));

    return 1;
}

static int udpBind(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state,
                      pds->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetUInteger(plua_state, 3, 0)));

    return 1;
}

static int udpRecvFrom(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));

    std::string ip("");
    unsigned short port(0);

    if (!pds)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pds->recvFrom(&ip, &port));

    luaPushString(plua_state, ip);
    luaPushUInteger(plua_state, port);

    return 3;
}

static int udpSendTo(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        if (LuaString == luaGetType(plua_state, 2))
        {
            std::string str = luaGetString(plua_state, 2);
            luaPushInteger(plua_state, pds->sendTo(str.c_str(),
                                                  luaGetUInteger(plua_state, 3, str.length()),
                                                  luaGetString(plua_state, 4, "127.0.0.1"),
                                                  luaGetUInteger(plua_state, 5, 0)));
        }
        else if (LuaLightUserData == luaGetType(plua_state, 2))
        {
            luaPushInteger(plua_state, pds->sendTo((const char*)luaGetLightUserData(plua_state, 2),
                                                  luaGetUInteger(plua_state, 3, 0),
                                                  luaGetString(plua_state, 4, "127.0.0.1"),
                                                  luaGetUInteger(plua_state, 5, 0)));
        }
        else
        {
            luaPushInteger(plua_state, -1);
        }
    }

    return 1;
}

static int udpGetRecvBuf(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pds)
        luaPushLightUserData(plua_state, (void*)0);
    else
        luaPushLightUserData(plua_state, (void*)pds->getRecvBuf());

    return 1;
}

static int udpClose(lua_State* plua_state)
{
    DgramSocket* pds = static_cast<DgramSocket*>(luaGetLightUserData(plua_state, 1, 0));

    if (!pds)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pds->closeSocket());

    return 1;
}

////////////////////////////////////////////////////
// TCP
////////////////////////////////////////////////////

static int tcpCreate(lua_State* plua_state)
{
    size_t family = luaGetUInteger(plua_state, 1, Family_IPv4);

    StreamSocket* pss = new StreamSocket((Family)family);

    LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pss, deleteVoid<StreamSocket>);

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
    if (!pss)
        luaPushInteger(plua_state, kInvalidSocket);
    else
        luaPushInteger(plua_state, pss->getSocket());

    return 1;
}

static int tcpInit(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pss->init());

    return 1;
}

static int tcpIsOk(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pss->isOk());

    return 1;
}

static int tcpSetBlock(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pss->setBlock(luaGetUInteger(plua_state, 2, 0)));

    return 1;
}

static int tcpBind(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state,
                      pss->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetUInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpListen(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pss->listen(luaGetUInteger(plua_state, 2, 5)));

    return 1;
}

static int tcpAccept(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        TIpAddrInfo tipaddrinfo;
        luaPushInteger(plua_state, pss->accept(&tipaddrinfo));
    }

    return 1;
}

static int tcpConnect(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state,
                      pss->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetUInteger(plua_state, 3, 0)));

    return 1;
}

static int tcpClientRecv(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pss->clientRecv());

    return 1;
}

static int tcpClientSend(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        if (LuaString == luaGetType(plua_state, 2))
        {
            std::string str = luaGetString(plua_state, 2);
            luaPushInteger(plua_state, pss->clientSend(str.c_str(),
                                                  luaGetUInteger(plua_state, 3, str.length())
                                                  ));
        }
        else if (LuaLightUserData == luaGetType(plua_state, 2))
        {
            luaPushInteger(plua_state, pss->clientSend((const char*)luaGetLightUserData(plua_state, 2),
                                                  luaGetUInteger(plua_state, 3, 0)
                                                  ));
        }
        else
        {
            luaPushInteger(plua_state, -1);
        }
    }

    return 1;
}

static int tcpServerRecv(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        int client_sock = luaGetInteger(plua_state, 2, kInvalidSocket);
        if (kInvalidSocket == client_sock)
        {
            luaPushInteger(plua_state, -1);
            return 1;
        }
        luaPushInteger(plua_state, pss->serverRecv(client_sock));
    }

    return 1;
}

static int tcpServerSend(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
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
                                                      luaGetUInteger(plua_state, 4, str.length())
                                                      ));
        }
        else if (LuaLightUserData == luaGetType(plua_state, 3))
        {
            luaPushInteger(plua_state, pss->serverSend(client_sock,
                                                    (const char*)luaGetLightUserData(plua_state, 3),
                                                    luaGetUInteger(plua_state, 4, 0)
                                                    ));
        }
        else
        {
            luaPushInteger(plua_state, -1);
        }
    }

    return 1;
}

static int tcpGetRecvBuf(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushLightUserData(plua_state, (void*)0);
    else
        luaPushLightUserData(plua_state, (void*)pss->getRecvBuf());

    return 1;
}

static int tcpClose(lua_State* plua_state)
{
    StreamSocket* pss = static_cast<StreamSocket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pss)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pss->closeSocket());

    return 1;
}

////////////////////////////////////////////////////
// RAW
////////////////////////////////////////////////////
static int rawCreate(lua_State* plua_state)
{
    Socket* ps = new Socket((Family)luaGetUInteger(plua_state, 1, Family_IPv4), SockType_Raw, luaGetUInteger(plua_state, 2, 0));

    LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)ps, deleteVoid<Socket>);

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
    if (!ps)
        luaPushInteger(plua_state, kInvalidSocket);
    else
        luaPushInteger(plua_state, ps->getSocket());

    return 1;
}

static int rawInit(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, ps->init());

    return 1;
}

static int rawIsOk(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, ps->isOk());

    return 1;
}

static int rawSetBlock(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, ps->setBlock(luaGetUInteger(plua_state, 2, 0)));

    return 1;
}

static int rawBind(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state,
                      ps->bind(luaGetString(plua_state, 2, ""), (unsigned short)luaGetUInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecvFrom(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 2, 0));
    size_t len = static_cast<size_t>(luaGetUInteger(plua_state, 3, 0));

    std::string ip("");
    unsigned short port(0);

    if (!ps)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, ps->recvFrom(buf, len, &ip, &port));

    luaPushString(plua_state, ip);
    luaPushUInteger(plua_state, port);

    return 3;
}

static int rawSendTo(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        if (LuaString == luaGetType(plua_state, 2))
        {
            std::string str = luaGetString(plua_state, 2);
            luaPushInteger(plua_state, ps->sendTo(str.c_str(),
                                                  luaGetUInteger(plua_state, 3, str.length()),
                                                  luaGetString(plua_state, 4, "127.0.0.1"),
                                                  luaGetUInteger(plua_state, 5, 0)));
        }
        else if (LuaLightUserData == luaGetType(plua_state, 2))
        {
            luaPushInteger(plua_state, ps->sendTo((const char*)luaGetLightUserData(plua_state, 2),
                                                  luaGetUInteger(plua_state, 3, 0),
                                                  luaGetString(plua_state, 4, "127.0.0.1"),
                                                  luaGetUInteger(plua_state, 5, 0)));
        }
        else
        {
            luaPushInteger(plua_state, -1);
        }
    }

    return 1;
}

static int rawListen(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, ps->listen(luaGetUInteger(plua_state, 2, 5)));

    return 1;
}

static int rawAccept(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
    {
        luaPushBoolean(plua_state, false);
        return 1;
    }
    else
    {
        TIpAddrInfo tipaddrinfo;
        luaPushBoolean(plua_state, true);
        std::string ip = ipArrayToStr((Family)tipaddrinfo.taddr.ipaddr_ver,
                         (unsigned char*)&tipaddrinfo.taddr.ipaddr,
                         16);

        luaPushString(plua_state, ip);
        luaPushUInteger(plua_state, tipaddrinfo.port);

        return 3;
    }
}

static int rawConnect(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state,
                      ps->connect(luaGetString(plua_state, 2, "127.0.0.1"), (unsigned short)luaGetUInteger(plua_state, 3, 0)));

    return 1;
}

static int rawRecv(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    char* buf = static_cast<char*>(luaGetLightUserData(plua_state, 3, 0));
    size_t len = static_cast<size_t>(luaGetUInteger(plua_state, 4, 0));

    if (!ps || kInvalidSocket == sock || NULL == buf)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, ps->recv(sock, buf, len));

    return 1;
}

static int rawSend(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));
    int sock = luaGetInteger(plua_state, 2, kInvalidSocket);
    if (!ps || kInvalidSocket == sock)
    {
        luaPushInteger(plua_state, -1);
    }
    else
    {
        if (LuaString == luaGetType(plua_state, 3))
        {
            std::string str = luaGetString(plua_state, 3);
            luaPushInteger(plua_state, ps->send(sock,
                                                str.c_str(),
                                                luaGetUInteger(plua_state, 4, str.length())
                                                ));
        }
        else if (LuaLightUserData == luaGetType(plua_state, 3))
        {
            luaPushInteger(plua_state, ps->send(sock,
                                                (const char*)luaGetLightUserData(plua_state, 3),
                                                luaGetUInteger(plua_state, 4, 0)
                                                ));
        }
        else
        {
            luaPushInteger(plua_state, -1);
        }
    }

    return 1;
}

static int rawClose(lua_State* plua_state)
{
    Socket* ps = static_cast<Socket*>(luaGetLightUserData(plua_state, 1, 0));

    if (!ps)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, ps->closeSocket());

    return 1;
}

//close_socket

static int close_socket(lua_State* plua_state)
{
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
