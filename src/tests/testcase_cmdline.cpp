#include "testcase_cmdline.hpp"
#include <vector>
#include "util/cmdline.hpp"

using namespace std;
using namespace util;

void TestCaseCmdLine::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseCmdLine, testCmdLineParser1)
    REGISTER_TEST_FUNCTION(TestCaseCmdLine, testCmdLineParser2)
    REGISTER_TEST_FUNCTION(TestCaseCmdLine, testCmdLineParser3)
    REGISTER_TEST_FUNCTION(TestCaseCmdLine, testCmdLineMaker)
}

void TestCaseCmdLine::setUp()
{
}

void TestCaseCmdLine::tearDown()
{    
}

void TestCaseCmdLine::testCmdLineParser1()
{
    string cmdline = "iperf -u -c 192.168.1.1 -b 5M -P 30 -t 60";
    CmdLineParser clp(true);
    assertTrue(clp.parse(cmdline), ASSERT_POSITION);
    assertFalse(clp.content().empty(), ASSERT_POSITION);
    assertEquals<string>(cmdline, clp.getCmdLine(), ASSERT_POSITION);
    assertEquals<string>("iperf", clp.getCmd(), ASSERT_POSITION);
    assertEquals<string>("192.168.1.1", clp.getValue("c"), ASSERT_POSITION);
    assertEquals<string>("5M", clp.getValue("b"), ASSERT_POSITION);
    assertEquals<int>(0, clp.getValue<int>("p", 0), ASSERT_POSITION);
    assertEquals<int>(30, clp.getValue<int>("P", 30), ASSERT_POSITION);
    assertEquals<int>(60, clp.getValue<int>("t", 60), ASSERT_POSITION);
    assertTrue(clp.hasKey("b"), ASSERT_POSITION);
    assertTrue(clp.hasKey("u"), ASSERT_POSITION);
    assertTrue(clp.hasKey("c"), ASSERT_POSITION);
    assertTrue(clp.hasKey("P"), ASSERT_POSITION);
    assertFalse(clp.hasKey("p"), ASSERT_POSITION);
    assertTrue(clp.hasKey("t"), ASSERT_POSITION);
    assertFalse(clp.hasKey("G"), ASSERT_POSITION);
    clp.removeKey("P");
    assertFalse(clp.hasKey("P"), ASSERT_POSITION);
    assertEquals<string>(cmdline, clp.getCmdLine(), ASSERT_POSITION);
}

void TestCaseCmdLine::testCmdLineParser2()
{
    string cmdline = "servicecmd -scmd \"iperf -u -c 192.168.1.1 -b 5M -P 30 -t 60\" -ip 192.168.1.12 -port 9001";
    CmdLineParser clp;
    assertTrue(clp.parse(cmdline), ASSERT_POSITION);
    assertFalse(clp.content().empty(), ASSERT_POSITION);
    assertEquals<string>(cmdline, clp.getCmdLine(), ASSERT_POSITION);
    assertEquals<string>("servicecmd", clp.getCmd(), ASSERT_POSITION);
    assertEquals<string>("192.168.1.12", clp.getValue("ip"), ASSERT_POSITION);
    assertEquals<int>(9001, clp.getValue<int>("port", 0), ASSERT_POSITION);
    assertEquals<string>("iperf -u -c 192.168.1.1 -b 5M -P 30 -t 60", clp.getValue("scmd"), ASSERT_POSITION);
}

void TestCaseCmdLine::testCmdLineParser3()
{
    CmdLineParser clp;
    assertFalse(clp.parse(""), ASSERT_POSITION);
}

void TestCaseCmdLine::testCmdLineMaker()
{
    CmdLineMaker clm(true, " -", " ");
    assertTrue(clm.isCaseSensitive(), ASSERT_POSITION);
    assertTrue(clm.getCmdLine().empty(), ASSERT_POSITION);
    assertTrue(clm.getCmd().empty(), ASSERT_POSITION);
    clm.setCmd("iperf");
    assertEquals<string>("iperf", clm.getCmd(), ASSERT_POSITION);
    assertFalse(clm.getCmdLine().empty(), ASSERT_POSITION);
    clm.addKeyValue<string>("u", "");
    clm.addKeyValue<string>("c", "192.168.1.1");
    clm.addKeyValue<string>("b", "5M");
    clm.addKeyValue<int>("P", 30);
    clm.addKeyValue<int>("t", 60);
    assertEquals<string>("iperf -u -c 192.168.1.1 -b 5M -P 30 -t 60", clm.getCmdLine(), ASSERT_POSITION);
    vector<string> vec;
    vec.push_back("t");
    vec.push_back("c");
    vec.push_back("b");
    clm.sort(vec.begin(), vec.end());
    assertEquals<string>("iperf -t 60 -c 192.168.1.1 -b 5M -u -P 30", clm.getCmdLine(), ASSERT_POSITION);
    clm.removeKey("c");
    assertEquals<string>("iperf -t 60 -b 5M -u -P 30", clm.getCmdLine(), ASSERT_POSITION);
}