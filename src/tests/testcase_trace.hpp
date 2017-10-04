#ifndef TESTS_TESTCASETRACE
#define TESTS_TESTCASETRACE

#include "util/unittest.hpp"

class TestCaseTrace : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseTrace)
    virtual void registerTestFunctions();
    
    //Tests
    void testTrace();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASETRACE