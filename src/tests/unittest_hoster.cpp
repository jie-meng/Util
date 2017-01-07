#include "unittest_hoster.hpp"
#include "testcase_string.hpp"
#include "testcase_memory.hpp"
#include "testcase_time.hpp"
#include  "testcase_typeinfo.hpp"
#include "testcase_file.hpp"
#include "testcase_lexicalcast.hpp"
#include "testcase_csv.hpp"
#include "testcase_cmdline.hpp"
#include "testcase_factory.hpp"
#include  "testcase_thread.hpp"
#include "testcase_timer.hpp"

using namespace std;
using namespace util;

UnitTestHoster::UnitTestHoster()
{
}

void UnitTestHoster::run()
{
    TestSuite suit;
    
    suit.addTestSuite(TestCaseString("TestCaseString"));
    suit.addTestSuite(TestCaseMemory("TestCaseMemory"));
    suit.addTestSuite(TestCaseTime("TestCaseTime"));
    suit.addTestSuite(TestCaseTypeInfo("TestCaseTypeInfo"));
    suit.addTestSuite(TestCaseFile("TestCaseFile"));
    suit.addTestSuite(TestCaseLexicalCast("TestCaseLexicalCast"));
    suit.addTestSuite(TestCaseCsv("TestCaseCsv"));
    suit.addTestSuite(TestCaseCmdLine("TestCaseCmdLine"));
    suit.addTestSuite(TestCaseFactory("TestCaseFactory"));
    suit.addTestSuite(TestCaseThread("TestCaseThread"));
    suit.addTestSuite(TestCaseTimer("TestCaseTimer"));

    TestRunner runner;
    runner.run(suit);
}
