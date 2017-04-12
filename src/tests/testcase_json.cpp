#include "testcase_json.hpp"
#include <vector>
#include "util/json.hpp"

using namespace std;
using namespace util;

void TestCaseJson::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseJson, testJsonToString)
    REGISTER_TEST_FUNCTION(TestCaseJson, testStringToJson)
}

void TestCaseJson::setUp()
{
}

void TestCaseJson::tearDown()
{    
}

void TestCaseJson::testJsonToString()
{
    assertEquals<string>("null", Json().toString(), ASSERT_POSITION);
    assertTrue(Json().isNull(), ASSERT_POSITION);
    assertEquals<string>("{}", Json::parse("{}").toString(), ASSERT_POSITION);
    assertTrue(Json::parse("{}").isObject(), ASSERT_POSITION);

    Json json;
    json.set("i", 22);
    json.set("s", "test");
    json.set("f", 3.14);
    json.set("v", vector<int>({10, 20, 30}));
    auto str = json.toString();

    assertTrue(strContains(str, "\"f\":3.14"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"i\":22"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"s\":\"test\""), ASSERT_POSITION);
    assertTrue(strContains(str, "\"v\":[10,20,30]"), ASSERT_POSITION);

    // json["i"] = 33;
    // assertEquals<int>(33, json["i"], ASSERT_POSITION);
}

void TestCaseJson::testStringToJson()
{

}