#include "testcase_trace.hpp"
#include "util/trace.hpp"

using namespace std;
using namespace util;

void TestCaseTrace::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseTrace, testTrace)
}

void TestCaseTrace::setUp()
{
}

void TestCaseTrace::tearDown()
{    
}

void TestCaseTrace::testTrace()
{
//    TraceMgr tracemgr;
//    if (!tracemgr.start())
//    {
//        cout<<"TraceMgr start failed"<<endl;
//        return -1;
//    }

//    StdoutListener sl;
//    tracemgr.addListener(&sl);

//    TcpListener tcpl;
//    if (tcpl.init("127.0.0.1", 60000))
//        tracemgr.addListener(&tcpl);
//    else
//        return 0;

//    UdpListener udpl;
//    if (udpl.init("127.0.0.1", 60001))
//        tracemgr.addListener(&udpl);
//    else
//        return 0;

//    FileListener filel;
//    if (filel.init("trace", 5000, "\n"))
//        tracemgr.addListener(&filel);

//    msleep(200);

//    for (int i=0; i<10000; ++i)
//        tracemgr.trace("os", Information::verbose, "msg %d", i);

//    getLine();

//    return 0;
}
