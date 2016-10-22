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
    assertTrue(isSpace(' '));
    assertTrue(isSpace('\r'));
    assertTrue(isSpace('\n'));
    assertTrue(isSpace('\t'));
    assertTrue(isSpace('\f'));
    assertTrue(isSpace('\v'));
}

void TestCaseString::testStrFormat()
{
    string s = strFormat("%d %s", 45, "glad");
    assertEquals<char>('4', s[0], "Fail on '4'");
    assertEquals<char>('5', s[1], "Fail on '5'");
    assertEquals<char>(' ', s[2], "Fail on ' '");
    assertEquals<char>('g', s[3], "Fail on 'g'");
    assertEquals<char>('l', s[4], "Fail on 'l'");
    assertEquals<char>('a', s[5], "Fail on 'a'");
    assertEquals<char>('d', s[6], "Fail on 'd'");
}

void TestCaseString::testStrToLower()
{
    string s = "ABCD123";
    s = strToLower(s);
    assertEquals<char>('a', s[0]);
    assertEquals<char>('b', s[1]);
    assertEquals<char>('c', s[2]);
    assertEquals<char>('d', s[3]);
    assertEquals<char>('1', s[4]);
}

void TestCaseString::testStrToUpper()
{
    string s = "axyz123";
    s = strToUpper(s);
    assertEquals<char>('A', s[0], "Fail on 'A'");
    assertEquals<char>('X', s[1], "Fail on 'X'");
    assertEquals<char>('Y', s[2], "Fail on 'Y'");
    assertEquals<char>('Z', s[3], "Fail on 'Z'");
    assertEquals<char>('1', s[4], "Fail on '1'");
}

void TestCaseString::testStrTrimLeftEx()
{
    string s = "<>kdy<xya";
    s = strTrimLeftEx(s, "<>");
    assertEquals<size_t>(7, s.length());
    assertEquals<char>('k', s[0]);
    assertEquals<char>('<', s[3]);
}

void TestCaseString::testStrTrimRightEx()
{
    string s = "<>kdy<xya";
    s = strTrimRightEx(s, "a");
    assertEquals<size_t>(8, s.length());
    assertEquals<char>('<', s[0]);
    assertEquals<char>('>', s[1]);
}

void TestCaseString::testStrTrimEx()
{
    string s = "<>kdy<xya<>>";
    s = strTrimEx(s, "<>");
    assertEquals<size_t>(7, s.length());
    assertEquals<char>('k', s[0], "k");
    assertEquals<char>('a', s[6], "a");
}
