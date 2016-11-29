#include "testcase_time.hpp"
#include "util/base.hpp"

using namespace std;
using namespace util;

void TestCaseTime::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseTime, testDateTime)
}

void TestCaseTime::setUp()
{
}

void TestCaseTime::tearDown()
{    
}

void TestCaseTime::testDateTime()
{
    tm t = localTime();
    DateTime dt(&t);
    printLine("%d-%d-%d %d:%d:%d %d", dt.getYear(), dt.getMonth(), dt.getDay(), dt.getHour(), dt.getMinute(), dt.getSecond(), dt.getWeekday());
}
