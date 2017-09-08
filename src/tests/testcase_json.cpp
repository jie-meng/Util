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

    Json sub;
    sub.set("k", 1);
    sub.set("v", 10);

    Json json;
    json.set("i", 22);
    json.set("s", "test");
    json.set("f", 3.14);
    json.set("b", true);
    json.setNull("n");
    json.set("v", vector<int>({10, 20, 30}));
    json.set("sub", sub);
    auto str = json.toString();

    assertTrue(strContains(str, "\"f\":3.14"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"i\":22"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"n\":null"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"s\":\"test\""), ASSERT_POSITION);
    assertTrue(strContains(str, "\"b\":true"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"v\":[10,20,30]"), ASSERT_POSITION);
    assertTrue(strContains(str, "\"sub\":{\"k\":1,\"v\":10}"), ASSERT_POSITION);

    assertTrue(json.get("i").isInt(), ASSERT_POSITION);
    assertTrue(json.get("s").isString(), ASSERT_POSITION);
    assertTrue(json.get("f").isFloat(), ASSERT_POSITION);
    assertTrue(json.get("b").isBool(), ASSERT_POSITION);
    assertTrue(json.get("n").isNull(), ASSERT_POSITION);
    assertTrue(json.get("v").isArray(), ASSERT_POSITION);
    assertTrue(json.get("sub").isObject(), ASSERT_POSITION);
    assertTrue(json.get("sub").get("k").isInt(), ASSERT_POSITION);
    assertTrue(json.get("sub").get("v").isInt(), ASSERT_POSITION);

    assertEquals<int>(22, json.get("i").toInt(), ASSERT_POSITION);
    assertEquals<string>("test", json.get("s").toString(), ASSERT_POSITION);
    assertEquals<float>(3.14, json.get("f").toFloat(), ASSERT_POSITION);
    assertEquals<bool>(true, json.get("b").toBool(), ASSERT_POSITION);
}

void TestCaseJson::testStringToJson()
{
    auto json = Json::parse("{\"f\":3.14,\"i\":22,\"b\":true,\"n\":null,\"s\":\"test\",\"sub\":{\"k\":1,\"v\":10},\"v\":[10,20,30]}");
    
    assertTrue(json.get("i").isInt(), ASSERT_POSITION);
    assertTrue(json.get("s").isString(), ASSERT_POSITION);
    assertTrue(json.get("f").isFloat(), ASSERT_POSITION);
    assertTrue(json.get("b").isBool(), ASSERT_POSITION);
    assertTrue(json.get("n").isNull(), ASSERT_POSITION);
    assertTrue(json.get("v").isArray(), ASSERT_POSITION);
    assertTrue(json.get("sub").isObject(), ASSERT_POSITION);
    assertTrue(json.get("sub").get("k").isInt(), ASSERT_POSITION);
    assertTrue(json.get("sub").get("v").isInt(), ASSERT_POSITION);

    assertEquals<int>(22, json.get("i").toInt(), ASSERT_POSITION);
    assertEquals<string>("test", json.get("s").toString(), ASSERT_POSITION);
    assertEquals<float>(3.14, json.get("f").toFloat(), ASSERT_POSITION);
    assertEquals<bool>(true, json.get("b").toBool(), ASSERT_POSITION);
    assertEquals<int>(1, json.get("sub").get("k").toInt(), ASSERT_POSITION);
    assertEquals<int>(10, json.get("sub").get("v").toInt(), ASSERT_POSITION);
    vector<Json> vec = json.get("v").toArray();
    assertEquals<int>(10, vec[0].toInt(), ASSERT_POSITION);
    assertEquals<int>(20, vec[1].toInt(), ASSERT_POSITION);
    assertEquals<int>(30, vec[2].toInt(), ASSERT_POSITION);
}