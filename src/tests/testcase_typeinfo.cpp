#include "testcase_typeinfo.hpp"
#include "util/typeinfo.hpp"

using namespace std;
using namespace util;

void TestCaseTypeInfo::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseTypeInfo, testTypeInfo)
}

void TestCaseTypeInfo::setUp()
{
}

void TestCaseTypeInfo::tearDown()
{    
}

void TestCaseTypeInfo::testTypeInfo()
{
    TypeInfo ti(typeid(int));
    assertEquals<string>(typeid(int).name(), ti.name(), ASSERT_POSITION);
    assertEquals<TypeInfo>(TypeInfo(typeid(10)), ti, ASSERT_POSITION);
    assertEquals<TypeInfo>(TypeInfo(typeid((float)10.2)), TypeInfo(typeid(float)), ASSERT_POSITION);
    assertEquals<TypeInfo>(TypeInfo(typeid(12.34)), TypeInfo(typeid(double)), ASSERT_POSITION);
    assertEquals<TypeInfo>(TypeInfo(typeid(string(""))), TypeInfo(typeid(string)), ASSERT_POSITION);
}
