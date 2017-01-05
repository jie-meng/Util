#ifndef TESTS_TESTCASEMEMORY
#define TESTS_TESTCASEMEMORY

#include "util/unittest.hpp"

class TestCaseMemory : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseMemory)
    virtual void registerTestFunctions();
    
    //Tests
    void testDelete();
    void testMemory();
    void testMemoryBuf();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEMEMORY
