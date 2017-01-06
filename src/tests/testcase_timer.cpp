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

class TestTimer : public Timer
{
public:
    int count = 0;
protected:
    virtual void runTask()
    {
        ++count;
    }
};

void TestCaseTimer::testTimer()
{
    TestTimer tt;
    assertTrue(tt.start(0, 200), ASSERT_POSITION);
    msleep(800);
    tt.stop();
    assertTrue(tt.count >= 2, ASSERT_POSITION);
}

void TestCaseTimer::bindFunc()
{
    ++count_;
}

void TestCaseTimer::testFuncTimer()
{
    count_ = 0;
    FuncTimer ft(UtilBind(&TestCaseTimer::bindFunc, this));
    assertTrue(ft.start(0, 200), ASSERT_POSITION);
    msleep(800);
    ft.stop();
    assertTrue(count_ >= 2, ASSERT_POSITION);    
}
