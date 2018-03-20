#ifndef TESTS_TESTCASETIME
#define TESTS_TESTCASETIME

#include "util/unittest.hpp"

class TestCaseTime : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseTime)
    virtual void registerTestFunctions();

    //Tests
    void testDateTime();
    void testGetCurrentMiliseconds();
protected:
    virtual void setUp();
    virtual void tearDown();
};

#endif //TESTS_TESTCASETIME
