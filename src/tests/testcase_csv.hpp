#ifndef TESTS_TESTCASECSV
#define TESTS_TESTCASECSV

#include "util/unittest.hpp"

class TestCaseCsv : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseCsv)
    virtual void registerTestFunctions();
    
    //Tests
    void testCsv();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASECSV
