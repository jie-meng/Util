#include "testcase_file.hpp"
#include "util/file.hpp"

using namespace std;
using namespace util;

const string kTempFileName = "temp_file";

void TestCaseFile::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseFile, testReadWrite)
}

void TestCaseFile::setUp()
{
}

void TestCaseFile::tearDown()
{
    if (isPathFile(kTempFileName))
        pathRemove(kTempFileName);
}

void TestCaseFile::testReadWrite()
{
    assertStringNotWhiteSpace(readTextFile("LICENSE"), ASSERT_POSITION);
    string content = "This is a test string.";
    assertTrue(writeTextFile(kTempFileName, content), ASSERT_POSITION);
    assertEquals(content, readTextFile(kTempFileName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempFileName, content), ASSERT_POSITION);
    assertEquals(content, readTextFile(kTempFileName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempFileName, content, true), ASSERT_POSITION);
    assertEquals(content + content, readTextFile(kTempFileName), ASSERT_POSITION);
}