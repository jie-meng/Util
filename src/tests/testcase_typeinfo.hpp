#ifndef TESTS_TESTCASETYPEINFO
#define TESTS_TESTCASETYPEINFO

#include "util/unittest.hpp"

class TestCaseTypeInfo : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseTypeInfo)
    virtual void registerTestFunctions();
    
    //Tests
    void testTypeInfo();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASETYPEINFO
