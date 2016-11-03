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
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEFILE
