#ifndef TESTS_TESTCASEFILE
#define TESTS_TESTCASEFILE

#include "util/unittest.hpp"

class TestCaseFile : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseFile)
    virtual void registerTestFunctions();
    
    //Tests
    void testReadWrite();
    void testCreateBinaryFile();
    void testWriteBinaryFile();
    void testFileInfo();
    void testPath();
    void testListFile();
    void testIsTextFile();
    void testMkDir();
    void testCopyFullPath();
    void testCopyTree();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEFILE
