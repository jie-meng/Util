#include "testcase_regex.hpp"
#include "util/regex.hpp"

using namespace std;
using namespace util;

void TestCaseRegex::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseRegex, testRegex)
    REGISTER_TEST_FUNCTION(TestCaseRegex, testRegexIgnoreCase)
    REGISTER_TEST_FUNCTION(TestCaseRegex, testRegexRightToLeft)
    REGISTER_TEST_FUNCTION(TestCaseRegex, testRegexSingleLine)
    REGISTER_TEST_FUNCTION(TestCaseRegex, testRegexMultiLine)
}

void TestCaseRegex::setUp()
{
}

void TestCaseRegex::tearDown()
{    
}

void TestCaseRegex::testRegex()
{
    Regex regex("[a-zA-Z]+");
    assertFalse(regex.match("0abc123"), ASSERT_POSITION);
    assertTrue(regex.search("0abc123"), ASSERT_POSITION);
    assertEquals<size_t>(1, regex.getMatchedGroupCnt(), ASSERT_POSITION);
    assertEquals<string>("abc", regex.getMatchedGroup(0), ASSERT_POSITION);
    regex.compile("[a-zA-Z]+(?<number>\\d)");
    assertTrue(regex.search("0abc12#3"), ASSERT_POSITION);
    assertEquals<size_t>(2, regex.getMatchedGroupCnt(), ASSERT_POSITION);
    assertEquals<string>("1", regex.getMatchedGroup(1), ASSERT_POSITION);
    assertEquals<string>("1", regex.getMatchedGroup("number"), ASSERT_POSITION);
}

void TestCaseRegex::testRegexIgnoreCase()
{
    Regex regex("test");
    assertFalse(regex.match("Test"), ASSERT_POSITION);
    regex.compile("test", Regex::IgnoreCase);
    assertTrue(regex.match("Test"), ASSERT_POSITION);
    assertEquals<string>("Test", regex.getMatchedGroup(0), ASSERT_POSITION);
}

void TestCaseRegex::testRegexRightToLeft()
{
    Regex regex("\\w+");
    assertTrue(regex.search("Test##OK"), ASSERT_POSITION);
    assertEquals<string>("Test", regex.getMatchedGroup(0), ASSERT_POSITION);
    regex.compile("\\w+", Regex::RightToLeft);
    assertTrue(regex.search("Test##OK"), ASSERT_POSITION);
    assertEquals<string>("OK", regex.getMatchedGroup(0), ASSERT_POSITION);
}

void TestCaseRegex::testRegexSingleLine()
{
    Regex regex(".+");
    assertTrue(regex.search("function \n\treturn"), ASSERT_POSITION);
    assertEquals<string>("function ", regex.getMatchedGroup(0), ASSERT_POSITION);
    regex.compile(".+", Regex::SingleLine);
    assertTrue(regex.search("function \n\treturn"), ASSERT_POSITION);
    assertEquals<string>("function \n\treturn", regex.getMatchedGroup(0), ASSERT_POSITION);
}

void TestCaseRegex::testRegexMultiLine()
{
    Regex regex("^.+$");
    assertFalse(regex.search("abcdefg\n123456"), ASSERT_POSITION);
    regex.compile("^.+$", Regex::MultiLine);
    assertTrue(regex.search("abcdefg\n123456"), ASSERT_POSITION);
    assertEquals<size_t>(1, regex.getMatchedGroupCnt(), ASSERT_POSITION);
    assertNotEquals<string>("abcdefg", regex.getMatchedGroup(0), ASSERT_POSITION);
}
