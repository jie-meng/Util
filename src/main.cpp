#include <iostream>
#include <map>
#include <list>
//#include <locale>
//#include <codecvt>
#include "util/thread.hpp"
#include "util/file.hpp"
#include "util/net.hpp"
#include "util/iterator.hpp"
#include "util/collection.hpp"
#include "util/cmdline.hpp"
#include "util/process.hpp"
#include "util/regex.hpp"
#include "util/timer.hpp"
#include "util/strategy.hpp"
#include "util/cfg.hpp"
#include "util/factory.hpp"
#include "util/unittest.hpp"
#include "util/luax.hpp"
#include "util/luaextend.hpp"
#include "util/any.hpp"
#include "util/trace.hpp"
#include "util/csv.hpp"
#include "tests/unittest_hoster.hpp"

using namespace std;
using namespace util;

void newConnFunc(size_t conn, TConnSocket* ptconn_socket)
{
    printLine("new conn %d", conn);
}

void recvFunc(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket)
{
    printLine("conn %d recv: %s", conn, buf);
}

void closeConnFunc(size_t conn)
{
    printLine("close conn %d", conn);
}

void serverThread(StreamSocket* pss)
{
    if(!pss->bind("", 9977))
    {
        printLine("bind fail!");
        return;
    }
    if (!pss->listen(5))
    {
        printLine("listen fail!");
        return;
    }
    printLine("start server now.");
    pss->startServer();
    printLine("server return");
    return;
}

void cmd(const std::string& cmd, StreamSocket* pss)
{
    pss->sendConn(1, cmd.c_str(), cmd.length());
}

class Ds
{
public:
    Ds(int x, int y) : x_(x), y_(y) {}
    ~Ds() {}
    inline int getX() const { return x_; }
    inline int getY() const { return y_; }
    friend std::istream& operator>>(std::istream& is, Ds& ds);
    friend std::ostream& operator<<(std::ostream& os, const Ds& ds);
private:
    int x_;
    int y_;
};

std::istream& operator>>(std::istream& is, Ds& ds)
{
    std::string str;
    is >> str;
    std::vector<std::string> vec;
    strSplit(str, "-", vec);
    if (vec.size() < 2)
        return is;
    ds.x_ = lexicalCastDefault<int>(vec[0], 0);
    ds.y_ = lexicalCastDefault<int>(vec[1], 0);

    return is;
}

std::ostream& operator<<(std::ostream& os, const Ds& ds)
{
    return os << ds.x_ << "-" << ds.y_;
}

void testSocket()
{
    //    StreamSocket ss(Family_IPv4,
//        UtilBind(&newConnFunc, _1, _2),
//        UtilBind(&recvFunc, _1, _2, _3, _4),
//        UtilBind(&closeConnFunc, _1));
//
//    Thread td(UtilBind(serverThread, &ss));
//    td.start();
//
//    CmdLine cl(UtilBind(cmd, _1, &ss));
//    cl.start();

}

class A
{
public:
    A() {}
    A(const std::string& g) : g_(g) {}
    ~A() {}
    std::string g() const { return g_; }
    virtual std::string name() const { return "A"; }
private:
    std::string g_;
};

class B : public A
{
public:
    B() {}
    ~B() {}
    virtual std::string name() const { return "B"; }
};

void testBind()
{
    StreamSocket ss(Family_IPv4);
    ss.init();
    if (!ss.bind("", 9967))
        printLine("fuck1");
    ss.closeSocket();
    ss.init();
    if(!ss.bind("", 9967))
        printLine("fuck2");
}

int create(lua_State* plua_state)
{
    luaPushLightUserData(plua_state, (void*)new int(luaGetInteger(plua_state, 1, 100)));

    return 1;
}

int destroy(lua_State* plua_state)
{
    int* pdata = (int*)luaGetLightUserData(plua_state, 1);
    if (pdata)
    {
        printLine(*pdata);
        delete pdata;
    }

    return 0;
}

class MySSListener : public StreamSocketSvrListener
{
public:
    MySSListener() {}
    virtual ~MySSListener() {}

    virtual void newConn(size_t conn, TConnSocket* ptconn_socket)
    {
        printLine("newConn %d", conn);
    }

    virtual void recvConn(char* buf, size_t len, size_t conn, TConnSocket* ptconn_socket)
    {
        printLine("recvConn %d : %s", conn, buf);
    }

    virtual void closeConn(size_t conn)
    {
        printLine("closeConn %d", conn);
    }

    virtual void processMsgConn(char* pmsg, size_t len, size_t conn, TConnSocket* ptconn_socket) {}
};

int Trace()
{
    TraceMgr tracemgr;
    if (!tracemgr.start())
    {
        cout<<"TraceMgr start failed"<<endl;
        return -1;
    }

    StdoutListener sl;
    tracemgr.addListener(&sl);

    TcpListener tcpl;
    if (tcpl.init("127.0.0.1", 60000))
        tracemgr.addListener(&tcpl);
    else
        return 0;

    UdpListener udpl;
    if (udpl.init("127.0.0.1", 60001))
        tracemgr.addListener(&udpl);
    else
        return 0;

    FileListener filel;
    if (filel.init("trace", 5000, "\n"))
        tracemgr.addListener(&filel);

    msleep(200);

    for (int i=0; i<10000; ++i)
        tracemgr.trace("os", Information::verbose, "msg %d", i);

    getLine();

    return 0;
}

#ifdef _CPP11_
void testCpp11()
{
    auto a = [](int x) { return x + 2; };

    UtilFunction<int (int)> s = a;

    printLine(s(4));

    auto x = std::make_tuple(1, 3, "shit", 4.5, "test");
    printLine(std::get<0>(x));
    printLine(std::get<4>(x));
}

void testProcess()
{
    Process p;
    auto ret = p.create("luaexec \"/home/joshua/tools/Lunar/s c/test.lua\"",
            "/home/joshua/tools/Lunar/scripts",
            true,
            true,
            [](const std::string& str){ printLine(str); });

    if (ret)
        printLine("process ok");
    else
        printLine("process error");
    auto s = getLine();
    if (p.isRunning())
    {
        p.input(s + "\n");
    }

    getLine();
}


int testCppCallLuaFunc()
{
    LuaState luaState;
    int err = luaState.parseFile("test.lua");
    if (0 != err)
    {
        printLine(luaGetError(luaState.getState(), err));
        return -1;
    }

    luaGetGlobal(luaState.getState(), "mog");
    err = luaCallFunc(luaState.getState(), 0, 3);
    if (0 != err)
    {
        printLine(luaGetError(luaState.getState(), err));
        luaPop(luaState.getState(), -1);
        return -1;
    }

    int ret_cnt = luaGetTop(luaState.getState());
    printLine(ret_cnt);
    printLine(luaGetInteger(luaState.getState(), 1));

    if (luaGetType(luaState.getState(), 2) == LuaType::LuaTable)
    {
        printLine("table elements:");
        auto vec = luaGetTable(luaState.getState(), 2);
        for (auto x : vec)
        {
            printLine("key: %s, value: %s", x.first.toString().c_str(), x.second.toString().c_str());
        }
    }

    printLine(luaGetString(luaState.getState(), 3));

    return 0;
}

#endif

int pnpoly (int nvert, float *vertx, float *verty, float testx, float testy) {

    int i, j, c = 0;

    for (i = 0, j = nvert-1; i < nvert; j = i++) {

        if ( ( (verty[i]>testy) != (verty[j]>testy) ) && (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
        {
            c = !c;
            printLine("in polygon " + toString(c));
        }
    }
    return c;
}

//const std::string ws2utf8( const std::wstring& src )
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//	return conv.to_bytes( src );
//}
//
//const std::wstring utf8_2_ws( const std::string& src )
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
//	return conv.from_bytes( src );
//}

void runUnitTest()
{
    UnitTestHoster::getInstance().run();
}

int main(int argc, char* argv[])
{
    if (argc > 1 && strAreEqual("$", argv[1]))
        runUnitTest();
    else
        luaExecutor(argc, argv);
    
    return 0;
}
