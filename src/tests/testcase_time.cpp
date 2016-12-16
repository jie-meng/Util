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
    //DateTime dt(localTime());
    //printLine("%d-%d-%d %d:%d:%d %d", dt.getYear(), dt.getMonth(), dt.getDay(), dt.getHour(), dt.getMinute(), dt.getSecond(), dt.getWeekday());
    
    //DateTime dt2(gmtTime());
    //printLine("%d-%d-%d %d:%d:%d %d", dt2.getYear(), dt2.getMonth(), dt2.getDay(), dt2.getHour(), dt2.getMinute(), dt2.getSecond(), dt2.getWeekday());
    
    //DateTime dt(2016, 12, 5, 20, 10, 30);
    DateTime dt;
    printLine(dt.format(""));
}
