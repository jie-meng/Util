#include "testcase_any.hpp"
#include "util/any.hpp"

using namespace std;
using namespace util;

void TestCaseAny::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseAny, testAny)
}

void TestCaseAny::setUp()
{
}

void TestCaseAny::tearDown()
{    
}

void TestCaseAny::testAny()
{
    any i(3);
    any d(3.4);
    any s(string("test"));
    
    assertEquals<string>("3", toString(i), ASSERT_POSITION);
    assertEquals<string>("3.4", toString(d), ASSERT_POSITION);
    assertEquals<string>("test", toString(s), ASSERT_POSITION);
    d = s;
    assertEquals<string>("test", toString(d), ASSERT_POSITION);
    assertEquals<string>("test", toString(s), ASSERT_POSITION);
}
