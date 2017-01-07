#ifndef TESTS_TESTCASEFACTORY
#define TESTS_TESTCASEFACTORY

#include "util/unittest.hpp"

class TestCaseFactory : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseFactory)
    virtual void registerTestFunctions();
    
    //Tests
    void testFactory();
    void testCloneFactory();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASEFACTORY
