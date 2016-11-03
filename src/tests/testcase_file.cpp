#include "testcase_file.hpp"
#include "util/file.hpp"

using namespace std;
using namespace util;

void TestCaseFile::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseFile, testReadWrite)
}

void TestCaseFile::setUp()
{
}

void TestCaseFile::tearDown()
{    
}

void TestCaseFile::testReadWrite()
{
    assertStringNotWhiteSpace(readTextFile("LICENSE"), ASSERT_POSITION);
}

