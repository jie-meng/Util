#include "testcase_string.hpp"

using namespace std;
using namespace util;

void TestCaseString::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseString, testIsSpace)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrFormat)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrToLower)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrToUpper)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrTrimLeftEx)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrTrimRightEx)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrTrimEx)
}

void TestCaseString::setUp()
{
}

void TestCaseString::tearDown()
{
}

void TestCaseString::testIsSpace()
{
    Assert::isTrue(isSpace(' '), ASSERT_POSITION);
    Assert::isTrue(isSpace('\r'), ASSERT_POSITION);
    Assert::isTrue(isSpace('\n'), ASSERT_POSITION);
    Assert::isTrue(isSpace('\t'), ASSERT_POSITION);
    Assert::isTrue(isSpace('\f'), ASSERT_POSITION);
    Assert::isTrue(isSpace('\v'), ASSERT_POSITION);
}

void TestCaseString::testStrFormat()
{
    string s = strFormat("%d %s", 45, "glad");
    Assert::areEqual('4', s[0], ASSERT_POSITION);
    Assert::areEqual('5', s[1], ASSERT_POSITION);
    Assert::areEqual(' ', s[2], ASSERT_POSITION);
    Assert::areEqual('g', s[3], ASSERT_POSITION);
    Assert::areEqual('l', s[4], ASSERT_POSITION);
    Assert::areEqual('a', s[5], ASSERT_POSITION);
    Assert::areEqual('d', s[6], ASSERT_POSITION);
}

void TestCaseString::testStrToLower()
{
    string s = "ABCD123";
    s = strToLower(s);
    Assert::areEqual('a', s[0], ASSERT_POSITION);
    Assert::areEqual('b', s[1], ASSERT_POSITION);
    Assert::areEqual('c', s[2], ASSERT_POSITION);
    Assert::areEqual('d', s[3], ASSERT_POSITION);
    Assert::areEqual('1', s[4], ASSERT_POSITION);
}

void TestCaseString::testStrToUpper()
{
    string s = "axyz123";
    s = strToUpper(s);
    Assert::areEqual('A', s[0], ASSERT_POSITION);
    Assert::areEqual('X', s[1], ASSERT_POSITION);
    Assert::areEqual('Y', s[2], ASSERT_POSITION);
    Assert::areEqual('Z', s[3], ASSERT_POSITION);
    Assert::areEqual('1', s[4], ASSERT_POSITION);
}

void TestCaseString::testStrTrimLeftEx()
{
    string s = "<>kdy<xya";
    s = strTrimLeftEx(s, "<>");
    Assert::areEqual<size_t>(7, s.length(), ASSERT_POSITION);
    Assert::areEqual('k', s[0], ASSERT_POSITION);
    Assert::areEqual('<', s[3], ASSERT_POSITION);
}

void TestCaseString::testStrTrimRightEx()
{
    string s = "<>kdy<xya";
    s = strTrimRightEx(s, "a");
    Assert::areEqual<size_t>(8, s.length(), ASSERT_POSITION);
    Assert::areEqual('<', s[0], ASSERT_POSITION);
    Assert::areEqual('>', s[1], ASSERT_POSITION);
}

void TestCaseString::testStrTrimEx()
{
    string s = "<>kdy<xya<>>";
    s = strTrimEx(s, "<>");
    Assert::areEqual<size_t>(7, s.length(), ASSERT_POSITION);
    Assert::areEqual('k', s[0], ASSERT_POSITION);
    Assert::areEqual('a', s[6], ASSERT_POSITION);
}
