#include "testcase_string.hpp"
#include <deque>

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
    REGISTER_TEST_FUNCTION(TestCaseString, testStrLeft)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrRight)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrMid)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrContains)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrCompare)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrAreEqual)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrJoin)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrSplit)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrSplitEx)
    REGISTER_TEST_FUNCTION(TestCaseString, testStrToCharArray)
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

void TestCaseString::testStrLeft()
{
    string s = "xyz123";
    assertEquals<string>("xyz", strLeft(s, 3), ASSERT_POSITION);
    assertStringEmpty(strLeft(s, 0), ASSERT_POSITION);
    assertEquals(s, strLeft(s, 10), ASSERT_POSITION);
}

void TestCaseString::testStrRight()
{
    string s = "xyz123";
    assertEquals<string>("123", strRight(s, 3), ASSERT_POSITION);
    assertStringEmpty(strRight(s, 0), ASSERT_POSITION);
    assertEquals(s, strRight(s, 10), ASSERT_POSITION);
}

void TestCaseString::testStrMid()
{
    string s = "xyz123";
    assertEquals<string>("12", strMid(s, 3, 2), ASSERT_POSITION);
    assertStringEmpty(strMid(s, 2, 0), ASSERT_POSITION);
    assertEquals(s, strMid(s, 0, 100), ASSERT_POSITION);
}

void TestCaseString::testStrContains()
{
    string s = "abcDXyZ123";
    assertTrue(strContains(s, "xyz", false), ASSERT_POSITION);
    assertFalse(strContains(s, "xyz"), ASSERT_POSITION);
    assertFalse(strContains(s, "xyz9", false), ASSERT_POSITION);
}

void TestCaseString::testStrCompare()
{
    string s1 = "bbcDXyZ123";
    string s2 = "cbcDXyZ123";
    string s3 = "abcDXyZ123";
    assertEquals(0, strCompare(s1, strToUpper(s1), false), ASSERT_POSITION);
    assertEquals(0, strCompare(s1, strToLower(s1), false), ASSERT_POSITION);
    assertNotEquals(0, strCompare(s1, strToUpper(s1)), ASSERT_POSITION);
    assertNotEquals(0, strCompare(s1, strToLower(s1)), ASSERT_POSITION);
    assertTrue(strCompare(s1, s2) < 0, ASSERT_POSITION);
    assertTrue(strCompare(s1, s2, false) < 0, ASSERT_POSITION);
    assertTrue(strCompare(s1, s3) > 0, ASSERT_POSITION);
    assertTrue(strCompare(s1, s3, false) > 0, ASSERT_POSITION);
}

void TestCaseString::testStrAreEqual()
{
    string s1 = "abcXyz123Op";
    assertTrue(strAreEqual(s1, strToLower(s1), false), ASSERT_POSITION);
    assertTrue(strAreEqual(s1, strToUpper(s1), false), ASSERT_POSITION);
    assertFalse(strAreEqual(s1, strToLower(s1)), ASSERT_POSITION);
    assertFalse(strAreEqual(s1, strToUpper(s1)), ASSERT_POSITION);
}

void TestCaseString::testStrJoin()
{
    deque<string> coll;
    coll.push_back("a");
    coll.push_back("b");
    coll.push_back("c");
    
    string s1 = strJoin(coll, ";,");
    assertEquals<string>("a;,b;,c", s1, ASSERT_POSITION);
    
    coll.push_front("x");
    string s2 = strJoin(coll.begin(), coll.end(), " - ?8& ()");
    assertEquals<string>("x - ?8& ()a - ?8& ()b - ?8& ()c", s2, ASSERT_POSITION);
}

void TestCaseString::testStrSplit()
{
    vector<string> coll;
    string s1 = "a;,b;,c";
    
    coll.clear();
    assertEquals<size_t>(3, strSplit(s1, ";,", coll), ASSERT_POSITION);
    assertEquals<string>("a", coll[0], ASSERT_POSITION);
    assertEquals<string>("b", coll[1], ASSERT_POSITION);
    assertEquals<string>("c", coll[2], ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(1, strSplit(s1, ";,", coll, 1), ASSERT_POSITION);
    assertEquals<string>("a;,b;,c", coll[0], ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(3, strSplit(s1, ";,", coll, 100), ASSERT_POSITION);
    assertEquals<string>("a", coll[0], ASSERT_POSITION);
    assertEquals<string>("b", coll[1], ASSERT_POSITION);
    assertEquals<string>("c", coll[2], ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(1, strSplit(s1, "?1", coll), ASSERT_POSITION);
    assertEquals<string>(s1, coll[0], ASSERT_POSITION);
    
    string s2 = "x - ?8& ()a - ?8& ()b - ?8& ()c";
    
    coll.clear();
    assertEquals<size_t>(4, strSplit(s2, " - ?8& ()", coll), ASSERT_POSITION);
    assertEquals<string>("x", coll[0], ASSERT_POSITION);
    assertEquals<string>("a", coll[1], ASSERT_POSITION);
    assertEquals<string>("b", coll[2], ASSERT_POSITION);
    assertEquals<string>("c", coll[3], ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(3, strSplit(s2, " - ?8& ()", coll, 3), ASSERT_POSITION);
    assertEquals<string>("x", coll[0], ASSERT_POSITION);
    assertEquals<string>("a", coll[1], ASSERT_POSITION);
    assertEquals<string>("b - ?8& ()c", coll[2], ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(1, strSplit(s2, "?1", coll), ASSERT_POSITION);
    assertEquals<string>(s2, coll[0], ASSERT_POSITION);
    
    string s3 = "a b  c   d";
    coll.clear();
    assertEquals<size_t>(7, strSplit(s3, " ", coll), ASSERT_POSITION);
    coll.clear();
    assertEquals<size_t>(0, strSplit(s3, "", coll), ASSERT_POSITION);
}

void TestCaseString::testStrSplitEx()
{
    string s1 = "ftp -src \"D:/Program files/data/test.bin\" -dst /home/usr/data/test.bin -ip 127.0.0.1 -port 6000";
    string s2 = "ftp -src [{<D:/Program files/data/test.bin>}] -dst /home/usr/data/test.bin -ip 127.0.0.1 -port 6000";
    vector<string> coll;
    
    coll.clear();
    assertEquals<size_t>(9, strSplitEx(s1, " ", "\"", "\"", coll), ASSERT_POSITION);
    coll.clear();
    assertEquals<size_t>(10, strSplit(s1, " ", coll), ASSERT_POSITION);
    coll.clear();
    assertEquals<size_t>(9, strSplitEx(s2, " ", "[{<", ">}]", coll), ASSERT_POSITION);
    coll.clear();
    assertEquals<size_t>(10, strSplit(s2, " ", coll), ASSERT_POSITION);
    
    coll.clear();
    assertEquals<size_t>(3, strSplitEx(s2, " ", "[{<", ">}]", coll, 3), ASSERT_POSITION);
    assertEquals<string>("ftp", coll[0], ASSERT_POSITION); 
    assertEquals<string>("-src", coll[1], ASSERT_POSITION);
    assertEquals<string>("[{<D:/Program files/data/test.bin>}] -dst /home/usr/data/test.bin -ip 127.0.0.1 -port 6000", coll[2], ASSERT_POSITION);
}

void TestCaseString::testStrToCharArray()
{
    string s = "abcdefg";
    char array[10];
    
    memset(array, 0, sizeof(array));
    assertNotNull<char>(strToCharArray(s, array, sizeof(array)), ASSERT_POSITION);
    assertEquals('a', array[0], ASSERT_POSITION);
    assertEquals('b', array[1], ASSERT_POSITION);
    assertEquals('c', array[2], ASSERT_POSITION);
    assertEquals('d', array[3], ASSERT_POSITION);
    assertEquals('e', array[4], ASSERT_POSITION);
    assertEquals('f', array[5], ASSERT_POSITION);
    assertEquals('g', array[6], ASSERT_POSITION);
    assertEquals('\0', array[7], ASSERT_POSITION);
    
    memset(array, 0, sizeof(array));
    assertNull<char>(strToCharArray(s, array, 4), ASSERT_POSITION);
    assertEquals('a', array[0], ASSERT_POSITION);
    assertEquals('b', array[1], ASSERT_POSITION);
    assertEquals('c', array[2], ASSERT_POSITION);
    assertEquals('\0', array[3], ASSERT_POSITION);
}
