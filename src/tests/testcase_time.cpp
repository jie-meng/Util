#include "testcase_time.hpp"
#include "util/base.hpp"
#include "util/thread.hpp"

using namespace std;
using namespace util;

void TestCaseTime::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseTime, testDateTime)
    REGISTER_TEST_FUNCTION(TestCaseTime, testGetCurrentMiliseconds)
}

void TestCaseTime::setUp()
{
}

void TestCaseTime::tearDown()
{    
}

void TestCaseTime::testDateTime()
{
    DateTime dt1(2016, 12, 5, 20, 10, 30);
    DateTime dt2(2016, 12, 5, 20, 10, 31);
    DateTime dt3(2016, 12, 5, 20, 11, 31);

    assertTrue(dt1 < dt2, ASSERT_POSITION);
    assertTrue(dt1 <= dt2, ASSERT_POSITION);
    assertTrue(dt2 > dt1, ASSERT_POSITION);
    assertTrue(dt2 >= dt1, ASSERT_POSITION);
    assertEquals<double>(1, dt2.drift(dt1), ASSERT_POSITION);
    assertEquals<double>(-1, dt1.drift(dt2), ASSERT_POSITION);
    assertEquals<double>(61, dt3.drift(dt1), ASSERT_POSITION);

    assertTrue(dt1.earlierThan(dt2), ASSERT_POSITION);
    assertTrue(dt2.earlierThan(dt3), ASSERT_POSITION);

    assertEquals<int>(2016, dt1.getYear(), ASSERT_POSITION);
    assertEquals<int>(12, dt1.getMonth(), ASSERT_POSITION);
    assertEquals<int>(5, dt1.getDay(), ASSERT_POSITION);
    assertEquals<int>(20, dt1.getHour(), ASSERT_POSITION);
    assertEquals<int>(10, dt1.getMinute(), ASSERT_POSITION);
    assertEquals<int>(30, dt1.getSecond(), ASSERT_POSITION);
    assertEquals<DateTime::Weekday>(DateTime::Monday, dt1.getWeekday(), ASSERT_POSITION);

    DateTime dt4(2017, 2, 2, 15, 34, 22);
    assertEquals<DateTime::Weekday>(DateTime::Thursday, dt4.getWeekday(), ASSERT_POSITION);
    assertEquals<int>(33, dt4.getYearday(), ASSERT_POSITION);
}

void TestCaseTime::testGetCurrentMiliseconds()
{
    uint64_t first = getCurrentMiliseconds();
    msleep(255);
    uint64_t second = getCurrentMiliseconds();

    int deviation = (second - first) - 255;
    assertTrue(deviation < 30 && deviation > -30, ASSERT_POSITION);
}
