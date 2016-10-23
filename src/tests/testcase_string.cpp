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
    REGISTER_TEST_FUNCTION(TestCaseString, testStrTrim)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrReplace)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrReplaceAll)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrStartWith)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrEndWith)
}

void TestCaseString::setUp()
{
}

void TestCaseString::tearDown()
{
}

void TestCaseString::testIsSpace()
{
    assertTrue(isSpace(' '), ASSERT_POSITION);
    assertTrue(isSpace('\r'), ASSERT_POSITION);
    assertTrue(isSpace('\n'), ASSERT_POSITION);
    assertTrue(isSpace('\t'), ASSERT_POSITION);
    assertTrue(isSpace('\f'), ASSERT_POSITION);
    assertTrue(isSpace('\v'), ASSERT_POSITION);
}

void TestCaseString::testStrFormat()
{
    string s = strFormat("%d %s", 45, "glad");
    assertEquals('4', s[0], ASSERT_POSITION);
    assertEquals('5', s[1], ASSERT_POSITION);
    assertEquals(' ', s[2], ASSERT_POSITION);
    assertEquals('g', s[3], ASSERT_POSITION);
    assertEquals('l', s[4], ASSERT_POSITION);
    assertEquals('a', s[5], ASSERT_POSITION);
    assertEquals('d', s[6], ASSERT_POSITION);
}

void TestCaseString::testStrToLower()
{
    string s = "ABCD123";
    s = strToLower(s);
    assertEquals('a', s[0], ASSERT_POSITION);
    assertEquals('b', s[1], ASSERT_POSITION);
    assertEquals('c', s[2], ASSERT_POSITION);
    assertEquals('d', s[3], ASSERT_POSITION);
    assertEquals('1', s[4], ASSERT_POSITION);
}

void TestCaseString::testStrToUpper()
{
    string s = "axyz123";
    s = strToUpper(s);
    assertEquals('A', s[0], ASSERT_POSITION);
    assertEquals('X', s[1], ASSERT_POSITION);
    assertEquals('Y', s[2], ASSERT_POSITION);
    assertEquals('Z', s[3], ASSERT_POSITION);
    assertEquals('1', s[4], ASSERT_POSITION);
}

void TestCaseString::testStrTrimLeftEx()
{
    string s = "<>kdy<xya";
    s = strTrimLeftEx(s, "<>");
    assertEquals<size_t>(7, s.length(), ASSERT_POSITION);
    assertEquals('k', s[0], ASSERT_POSITION);
    assertEquals('<', s[3], ASSERT_POSITION);
}

void TestCaseString::testStrTrimRightEx()
{
    string s = "<>kdy<xya";
    s = strTrimRightEx(s, "a");
    assertEquals<size_t>(8, s.length(), ASSERT_POSITION);
    assertEquals('<', s[0], ASSERT_POSITION);
    assertEquals('>', s[1], ASSERT_POSITION);
}

void TestCaseString::testStrTrimEx()
{
    string s = "<>kdy<xya<>>";
    s = strTrimEx(s, "<>");
    assertEquals<size_t>(7, s.length(), ASSERT_POSITION);
    assertEquals('k', s[0], ASSERT_POSITION);
    assertEquals('a', s[6], ASSERT_POSITION);
}

void TestCaseString::testStrTrim()
{
    string s = "      tx ky  \t\n\r";
    s = strTrim(s);
    assertEquals<string>("tx ky", s, ASSERT_POSITION);
}

void TestCaseString::testStrReplace()
{
    string s = "dabcdabc";
    s = strReplace(s, "abc", "xyz");
    assertEquals<string>("dxyzdabc", s, ASSERT_POSITION);
}

void TestCaseString::testStrReplaceAll()
{
    string s = "dabcdabc";
    s = strReplaceAll(s, "abc", "xyz");
    assertEquals<string>("dxyzdxyz", s, ASSERT_POSITION);
}

void TestCaseString::testStrStartWith()
{
    string s = " xyzda";
    assertTrue(strStartWith(s, " x"), ASSERT_POSITION);
}

void TestCaseString::testStrEndWith()
{
    string s = " xyzda";
    assertTrue(strEndWith(s, "da"), ASSERT_POSITION);
}