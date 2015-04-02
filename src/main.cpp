#include <iostream>
#include <map>
#include <list>
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
#include "util/matrix.hpp"

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

class DemoTestCase : public TestCase
{
public:
    TESTCASE_COMMON(DemoTestCase)

    virtual void registerTestFunctions()
    {
        REGISTER_TEST_FUNCTION(DemoTestCase, test1)
        REGISTER_TEST_FUNCTION(DemoTestCase, test2)
        REGISTER_TEST_FUNCTION(DemoTestCase, test3)
        REGISTER_TEST_FUNCTION(DemoTestCase, test4);
    }

    void test1() throw(std::exception) { int a = 3; int* x = &a; assertNull(x, "test1 exception"); }
    void test2() throw(std::exception) { int p = 0; assertNotNull(&p, "test2 exception"); }
    void test3() throw(std::exception) { assertEquals(1, 1, "test3 exception"); }
    void test4() throw(std::exception) { throw std::exception(); }
protected:
    virtual void setUp() throw (std::exception) {}
    virtual void tearDown() throw (std::exception) {}
};

void testUnitTest()
{
    TestSuite suit;
    DemoTestCase demo_case("DemoTestCase");
    suit.addTestSuite(demo_case);
    suit.addTest(demo_case.create("test1"));

    TestRunner runner;
    //runner.setPrinter(UtilAutoPtr<Printer>(new UdpPrinter(60001)));
    runner.run(suit);
}

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

void testInv()
{
    Matrix m(7, 7);

    m(0, 0) =      6;
    m(0, 1) =      0;
    m(0, 2) =     0;
    m(0, 3) =      330;
    m(0, 4) =      0;
    m(0, 5) =      -594;
    m(0, 6) =      336;

    m(1, 0) =      0;
    m(1, 1) =      6;
    m(1, 2) =      0;
    m(1, 3) =      336;
    m(1, 4) =      594;
    m(1, 5) =      0;
    m(1, 6) =      -330;

    m(2, 0) =      0;
    m(2, 1) =      0;
    m(2, 2) =      6;
    m(2, 3) =      594;
    m(2, 4) =      -336;
    m(2, 5) =      330;
    m(2, 6) =      0;

    m(3, 0) =      330;
    m(3, 1) =      336;
    m(3, 2) =      594;
    m(3, 3) =      95772;
    m(3, 4) =      0;
    m(3, 5) =      0;
    m(3, 6) =      0;

    m(4, 0) =      0;
    m(4, 1) =      594;
    m(4, 2) =      -336;
    m(4, 3) =      0;
    m(4, 4) =      77622;
    m(4, 5) =      -18480;
    m(4, 6) =      -32670;

    m(5, 0) =      -594;
    m(5, 1) =      0;
    m(5, 2) =      330;
    m(5, 3) =      0;
    m(5, 4) =      -18480;
    m(5, 5) =      76956;
    m(5, 6) =      -33264;

    m(6, 0) =      336;
    m(6, 1) =      -330;
    m(6, 2) =      0;
    m(6, 3) =      0;
    m(6, 4) =      -32670;
    m(6, 5) =      -33264;
    m(6, 6) =      36966;

    printLine(m);

    if (m.invertable())
        printLine(m.inverse());
}

void testInv2()
{
    Matrix m(3, 3);

    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;

    m(1, 0) = 4;
    m(1, 1) = 5;
    m(1, 2) = 6;

    m(2, 0) = 7;
    m(2, 1) = 8;
    m(2, 2) = 9;

    printLine(m);

    if (m.invertable())
        printLine(m.inverse());
}

void testInv3()
{
    Matrix m(2, 2);

    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    printLine(m);

    if (m.invertable())
        printLine(m.inverse());
}

void testRank()
{
    Matrix m(2, 2);

    m(0, 0) = 1.5;
    m(0, 1) = 2;
    m(1, 0) = 3.1;
    m(1, 1) = 4;

    printLine(m.rank());
    printLine(m.invertable());
}

void testAddValue()
{
    Matrix m(2, 2);
    double d[4] = { 2, 4, 6, 7 };
    m.setElements(&d[0], &d[4]);
    printLine(m);
}

void testParamCalcData()
{
    Matrix x(7, 7);
    double values[49] = {3,             0,             0, -6.33173e+006,             0, -1.08342e+007,  1.44183e+007,
            0,             3,             0,  1.44183e+007,  1.08342e+007,             0,  6.33173e+006,
            0,             0,             3,  1.08342e+007, -1.44183e+007, -6.33173e+006,             0,
-6.33173e+006,  1.44183e+007,  1.08342e+007,  1.21786e+014,             0,             0,             0,
            0,  1.08342e+007, -1.44183e+007,             0,  1.08422e+014,  3.04308e+013,  2.28665e+013,
-1.08342e+007,             0, -6.33173e+006,             0,  3.04308e+013,  5.24906e+013, -5.20703e+013,
 1.44183e+007,  6.33173e+006,             0,             0,  2.28665e+013, -5.20703e+013,  8.26589e+013 };

    x.setElements(&values[0], &values[49]);

    printLine(x.rank());
    printLine(x.invertable());
}

#ifdef _CPP11_
void testCpp11()
{
    auto a = [](int x) { return x + 2; };

    UtilFunction<int (int)> s = a;

    printLine(s(4));
}
#endif

int main(int argc, char* argv[])
{
    luaExecutor(argc, argv);

    return 0;
}
