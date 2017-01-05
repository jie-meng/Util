#include "testcase_timer.hpp"
#include "util/timer.hpp"

using namespace std;
using namespace util;

void TestCaseTimer::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseTimer, testTimer)
    REGISTER_TEST_FUNCTION(TestCaseTimer, testFuncTimer)
}

void TestCaseTimer::setUp()
{
}

void TestCaseTimer::tearDown()
{    
}

void TestCaseTimer::testTimer()
{
    
}

void TestCaseTimer::testFuncTimer()
{
    
}
