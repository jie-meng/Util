#include "testcase_file.hpp"
#include "util/file.hpp"

using namespace std;
using namespace util;

const string kTempFileName = "temp_file";

void TestCaseFile::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseFile, testReadWrite)
    REGISTER_TEST_FUNCTION(TestCaseFile, testCreateBinaryFile)
    REGISTER_TEST_FUNCTION(TestCaseFile, testWriteBinaryFile)
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

void TestCaseFile::testCreateBinaryFile()
{
    assertTrue(createBinaryFile(kTempFileName, 64, 'A'), ASSERT_POSITION);
    assertEquals<uint64_t>(64, fileSize(kTempFileName), ASSERT_POSITION);
    char buf[64];
    memset(buf, 0, sizeof(buf));
    assertTrue(readBinaryFile(kTempFileName, buf, 64), ASSERT_POSITION);
    assertEquals<char>('A', buf[45], ASSERT_POSITION);
    
    memset(buf, 0, sizeof(buf));
    assertTrue(readBinaryFile(kTempFileName, buf, 32, 32), ASSERT_POSITION);
}

void TestCaseFile::testWriteBinaryFile()
{
    char buf[64];
    char read[128];
    
    //?
    memset(buf, '?', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf)), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf), fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('?', read[22], ASSERT_POSITION);
    
    //#
    memset(buf, '#', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf)), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf), fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    
    //$
    memset(buf, '$', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf), true), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf) * 2, fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    assertEquals<char>('$', read[65], ASSERT_POSITION);
    
    //^
    memset(buf, '^', sizeof(buf));
    assertTrue(overwriteBinaryFile(kTempFileName, buf, sizeof(buf), 32), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    assertEquals<char>('^', read[55], ASSERT_POSITION);
    assertEquals<char>('^', read[73], ASSERT_POSITION);
    assertEquals<char>('$', read[110], ASSERT_POSITION);
}
