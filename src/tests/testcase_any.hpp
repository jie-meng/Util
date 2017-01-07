#ifndef TESTS_TESTCASEANY
#define TESTS_TESTCASEANY

#include "util/unittest.hpp"

class TestCaseAny : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseAny)
    virtual void registerTestFunctions();
    
    //Tests
    void testAny();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEANY
